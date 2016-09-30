/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#include <protocol_session/Session.hpp>
#include <protocol_session/TorrentPieceInformation.hpp>
#include <protocol_session/Exceptions.hpp>
#include <protocol_session/detail/Buying.hpp>
#include <protocol_session/detail/Selling.hpp>
#include <protocol_session/detail/Observing.hpp>
#include <common/Bitcoin.hpp> // BITCOIN_DUST_LIMIT
#include <common/P2SHAddress.hpp>

#include <numeric>

namespace joystream {
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    Buying<ConnectionIdType>::Buying(Session<ConnectionIdType> * session,
                                     const RemovedConnectionCallbackHandler<ConnectionIdType> & removedConnection,
                                     const GenerateP2SHKeyPairCallbackHandler &generateP2SHKeyPair,
                                     const GenerateReceiveAddressesCallbackHandler &generateReceiveAddresses,
                                     const GenerateChangeAddressesCallbackHandler &generateChangeAddresses,
                                     const BroadcastTransaction & broadcastTransaction,
                                     const FullPieceArrived<ConnectionIdType> & fullPieceArrived,
                                     const Coin::UnspentOutputSet & funding,
                                     const BuyingPolicy & policy,
                                     const protocol_wire::BuyerTerms & terms,
                                     const TorrentPieceInformation & information)
        : _session(session)
        , _removedConnection(removedConnection)
        , _generateP2SHKeyPair(generateP2SHKeyPair)
        , _generateReceiveAddresses(generateReceiveAddresses)
        , _generateChangeAddresses(generateChangeAddresses)
        , _broadcastTransaction(broadcastTransaction)
        , _fullPieceArrived(fullPieceArrived)
        , _funding(funding)
        , _policy(policy)
        , _state(BuyingState::sending_invitations)
        , _terms(terms)
        , _numberOfMissingPieces(0)
        , _assignmentLowerBound(0) {
        //, _lastStartOfSendingInvitations(0) {

        // Setup pieces
        for(uint i = 0;i < information.size();i++) {

            PieceInformation p = information[i];

            _pieces.push_back(detail::Piece<ConnectionIdType>(i, p));

            if(!p.downloaded())
                _numberOfMissingPieces++;
        }

        // Notify any existing peers
        for(auto i : _session->_connections)
            (i.second)->processEvent(protocol_statemachine::event::BuyModeStarted(_terms));

        // If session is started, then set start time of this new mode
        if(_session->_state == SessionState::started)
            _lastStartOfSendingInvitations = std::chrono::high_resolution_clock::now();
    }

    template <class ConnectionIdType>
    uint Buying<ConnectionIdType>::addConnection(const ConnectionIdType & id, const SendMessageOnConnection & callback) {

        // Create connection
        detail::Connection<ConnectionIdType> * connection = _session->createAndAddConnection(id, callback);

        // Choose mode on connection
        connection->processEvent(protocol_statemachine::event::BuyModeStarted(_terms));

        return _session->_connections.size();
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::removeConnection(const ConnectionIdType & id) {

        // We explicitly check for stopped session, although checking for spesific connection existance
        // implicitly covers this case. It improves feedback to client.
        if(_session->_state == SessionState::stopped)
            throw exception::StateIncompatibleOperation("cannot remove connection while stopped, all connections are removed");

        if(!_session->hasConnection(id))
            throw exception::ConnectionDoesNotExist<ConnectionIdType>(id);

        removeConnection(id, DisconnectCause::client);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::validPieceReceivedOnConnection(const ConnectionIdType & id, int index) {

        // No state guard required, piece validation can occur
        // after connection is gone, or even session has been stopped.

        // Update status of seller
        auto itr = _sellers.find(id);
        assert(itr != _sellers.cend());

        detail::Seller<ConnectionIdType> & s = itr->second;

        if(s.indexOfAssignedPiece() != index)
            throw exception::IncorrectPieceIndex(index, s.indexOfAssignedPiece());

        // This results in payment being sent,
        // if connection is still live, and state updated
        s.pieceWasValid();

        // Update piece status
        detail::Piece<ConnectionIdType> & piece = _pieces[index];
        assert(piece.connectionId() == id);
        assert(piece.state() != PieceState::unassigned);

        // if its not already downloaded from out of bounds
        if(piece.state() != PieceState::downloaded) {

            assert(piece.state() == PieceState::being_validated_and_stored);
            assert(_state == BuyingState::downloading);

            // Mark as downloaded
            piece.downloaded();

            // Count downloaded piece
            _numberOfMissingPieces--;

            // Do we still have pieces which are not yet downloaded
            if(_numberOfMissingPieces > 0) {

                // If so, and are stills started,
                // then we try to assign a piece to this seller
                if(_session->_state == SessionState::started)
                    tryToAssignAndRequestPiece(s);

            } else if(_numberOfMissingPieces == 0) // otherwise we are done!
                _state = BuyingState::download_completed;

        }

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::invalidPieceReceivedOnConnection(const ConnectionIdType & id, int index) {

        // No state guard required, piece validation can occur
        // after connection is gone, or even session has been stopped.

        // Update status of seller
        auto itr = _sellers.find(id);
        assert(itr != _sellers.cend());

        detail::Seller<ConnectionIdType> & s = itr->second;
        if(s.indexOfAssignedPiece() != index)
            throw exception::IncorrectPieceIndex(index, s.indexOfAssignedPiece());

        s.pieceWasInvalid();

        // If connection still exists, remove it,
        // however it may no longer exist,
        // e.g. because it left, or we got stopped.
        if(_session->hasConnection(id))
            removeConnection(id, DisconnectCause::seller_sent_invalid_piece);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::peerAnnouncedModeAndTerms(const ConnectionIdType & id, const protocol_statemachine::AnnouncedModeAndTerms & a) {

        assert(_session->_state != SessionState::stopped);
        assert(_session->hasConnection(id));

        detail::Connection<ConnectionIdType> * c = _session->get(id);

        //assert(c->announcedModeAndTermsFromPeer() == a);

        // If we are currently started and sending out invitations, then we may (re)invite
        // sellers with sufficiently good terms
        if(_session->_state == SessionState::started &&
           _state == BuyingState::sending_invitations) {

            // Check that this peer is seller,
            protocol_statemachine::ModeAnnounced m = a.modeAnnounced();
            assert(m != protocol_statemachine::ModeAnnounced::none);

            // and has good enough terms to warrant an invitation,
            // then send invitation
            if(m == protocol_statemachine::ModeAnnounced::sell && _terms.satisfiedBy(a.sellModeTerms())) {
                c->processEvent(protocol_statemachine::event::InviteSeller());
                std::cout << "Invited: " << IdToString(id) << std::endl;
            }
        }
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::sellerHasJoined(const ConnectionIdType & id) {

        // Cannot happen when stopped, as there are no connections
        assert(_session->_state != SessionState::stopped);
        assert(_session->hasConnection(id));

        if(_session->_state == SessionState::started &&
           _state == BuyingState::sending_invitations)
            tryToStartDownloading();
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::sellerHasInterruptedContract(const ConnectionIdType & id) {

        // Cannot happen when stopped, as there are no connections
        assert(_session->_state != SessionState::stopped);
        assert(_session->hasConnection(id));

        // Get reference to seller corresponding connection
        auto itr = _sellers.find(id);
        assert(itr != _sellers.cend());

        // Remove connection
        removeConnection(id, DisconnectCause::seller_has_interrupted_contract);

        // Notify state machine about deletion
        throw protocol_statemachine::exception::StateMachineDeletedException();
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::receivedFullPiece(const ConnectionIdType & id, const protocol_wire::PieceData & p) {

        // Cannot happen when stopped, as there are no connections
        assert(_session->_state != SessionState::stopped);
        assert(_state == BuyingState::downloading);

        /**
         * Invariant _state == BuyingState::downloading
         * Is upheld by the fact that whenever a piece is assigned
         * a new piece, any peer which may have previosuyl have had it
         * assigned will ahve been removed, e.g. due to time out.
         */

        // Get seller corresponding to given id
        auto itr = _sellers.find(id);
        assert(itr != _sellers.end());

        detail::Seller<ConnectionIdType> & s = itr->second;

        assert(s.state() == SellerState::waiting_for_full_piece);

        // Update state
        s.fullPieceArrived();

        // Get piece and update status
        int index = s.indexOfAssignedPiece();

        detail::Piece<ConnectionIdType> & piece = _pieces[index];

        piece.arrived();

        /**
        // NB: let client deal with this for now
        // Check that this is correct length
        if(p.length() != piece.size()) {

            // 1) tell client
            // 2) remove connection
        }
        */

        // Notify client
        _fullPieceArrived(id, p, index);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::leavingState() {

        // Prepare sellers before we interrupt with new mode
        politeSellerCompensation();
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::start() {

        assert(_session->_state != SessionState::started);

        // Note starting time
        _lastStartOfSendingInvitations = std::chrono::high_resolution_clock::now();

        // Set client mode to started
        _session->_state = SessionState::started;
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::stop() {

        assert(_session->_state != SessionState::stopped);

        // Prepare sellers for closing connections
        politeSellerCompensation();

        // Clear sellers
        _sellers.clear();

        // Disconnect everyone:
        for(auto itr = _session->_connections.cbegin();itr != _session->_connections.cend();)
            itr = removeConnection(itr->first, DisconnectCause::client);

        // Update state
        _session->_state = SessionState::stopped;
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::pause() {

        // We can only pause if presently started
        if(_session->_state == SessionState::paused ||
           _session->_state == SessionState::stopped)
            throw exception::StateIncompatibleOperation("cannot pause while already paused/stopped.");

        // Update state
        _session->_state = SessionState::paused;
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::tick() {

        // Only process if we are active
        if(_session->_state == SessionState::started) {

            if(_state == BuyingState::sending_invitations)
                tryToStartDownloading();
            else if(_state == BuyingState::downloading) {

                for(auto mapping : _sellers) {

                    // Reference to seller
                    detail::Seller<ConnectionIdType> & s = mapping.second;

                    // A seller may be waiting to be assigned a new piece
                    if(s.state() == SellerState::waiting_to_be_assigned_piece) {

                        // This can happen when a seller has previously uploaded a valid piece,
                        // but there were no unassigned pieces at that time,
                        // however they become unassigned later as result of:
                        // * time out of old seller
                        // * seller interrupts contract by updating terms
                        // * seller sent an invalid piece

                        tryToAssignAndRequestPiece(s);

                    } else if(s.state() == SellerState::waiting_for_full_piece) {

                        // Get id of connection for this seller
                        ConnectionIdType id = s.connection()->connectionId();

                        // Check if seller has timed out in servicing the current request,
                        // if so remove connection
                        if(s.servicingPieceHasTimedOut(_policy.servicingPieceTimeOutLimit()))
                            removeConnection(id, DisconnectCause::seller_servicing_piece_has_timed_out);
                    }
                }
            }
        }
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::pieceDownloaded(int index) {

        assert(index >= 0);

        // We cannot apriori assert anything about piece state.
        detail::Piece<ConnectionIdType> & piece = _pieces[index];

        // If its not already, then mark piece as downloaded and
        // count towards missing piece count.
        // NB: There may be a seller currently sending us this piece,
        // or it may be in validation/storage, or even downloaded before.

        if(piece.state() != PieceState::downloaded) {

            _numberOfMissingPieces--;

            // This may be the last piece
            if(_numberOfMissingPieces == 0)
                _state = BuyingState::download_completed;
        }

        piece.downloaded();

        // Remove from unassigned queue if present
        _deAssignedPieces.erase(std::remove(_deAssignedPieces.begin(), _deAssignedPieces.end(), index));
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::updateTerms(const protocol_wire::BuyerTerms & terms) {

        // If we change terms when we are actually downloading,
        // then politely compensate sellers
        if(_state == BuyingState::downloading)
            politeSellerCompensation();

        // Notify existing peers
        for(auto itr : _session->_connections)
            itr.second->processEvent(protocol_statemachine::event::UpdateTerms<protocol_wire::BuyerTerms>(terms));

        // Set new terms
        _terms = terms;

        // If the download was not yet completed
        if(_state != BuyingState::download_completed) {

            // start over sending invitations
            _state = BuyingState::sending_invitations;

            // ditch any existing sellers
            _sellers.clear();

            for(auto mapping : _session->_connections) {

                detail::Connection<ConnectionIdType> * c = mapping.second;

                // Check that this peer is seller,
                protocol_statemachine::AnnouncedModeAndTerms a = c->announcedModeAndTermsFromPeer();

                // and has good enough terms to warrant an invitation,
                // then send invitation
                if(a.modeAnnounced() == protocol_statemachine::ModeAnnounced::sell && _terms.satisfiedBy(a.sellModeTerms())) {

                    c->processEvent(protocol_statemachine::event::InviteSeller());

                    std::cout << "Invited: " << IdToString(mapping.first) << std::endl;
                }
            }
        }
    }

    template <class ConnectionIdType>
    typename status::Buying<ConnectionIdType> Buying<ConnectionIdType>::status() const {

        // Generate statuses of all pieces
        std::vector<status::Piece<ConnectionIdType>> pieceStatuses;

        // SKIPPING DUE TO SPEED
        //for(auto piece : _pieces)
        //    pieceStatuses.push_back(piece.status());

        // Generate statuses of all sellers
        std::map<ConnectionIdType, status::Seller<ConnectionIdType>> sellerStatuses;

        for(auto mapping : _sellers) {
            // skip sellers that are no longer around
            if(mapping.second.state() == protocol_session::SellerState::gone)
                continue;

            sellerStatuses.insert(std::make_pair(mapping.first, mapping.second.status()));
        }

        return status::Buying<ConnectionIdType>(_funding,
                                                _policy,
                                                _state,
                                                _terms,
                                                sellerStatuses,
                                                _contractTx,
                                                pieceStatuses);
    }

    template <class ConnectionIdType>
    bool Buying<ConnectionIdType>::canToStartDownloading() {

        assert(_session->_state == SessionState::started);
        assert(_state == BuyingState::sending_invitations);
        assert(_sellers.empty());

        auto t0 = std::chrono::high_resolution_clock::now();

        auto timeSinceSendingInvitations = t0 - _lastStartOfSendingInvitations;

        return timeSinceSendingInvitations >= _policy.minTimeBeforeBuildingContract();
    }


    template <class ConnectionIdType>
    bool Buying<ConnectionIdType>::tryToStartDownloading() {

        std::cout << "Trying to start downloading." << std::endl;

        assert(_session->_state == SessionState::started);
        assert(_state == BuyingState::sending_invitations);
        assert(_sellers.empty());

        // Determine if we can start to build contract at present
        if(!canToStartDownloading()) {

            std::cout << "Aborted, cannot start at current time." << std::endl;
            return false;
        }

        // Select connections of peers to sell to
        std::vector<detail::Connection<ConnectionIdType> *> selected = selectSellers();

        uint32_t numberOfSellers = selected.size();

        // Make sure there are enough peers
        if(numberOfSellers < _terms.minNumberOfSellers()) {

            std::cout << "Aborted, too few viable seller peers, needed " << _terms.minNumberOfSellers() << ", found " << numberOfSellers << std::endl;
            return false;
        }

        /////////////////////////

        // Determine fund distribution among sellers
        std::vector<protocol_wire::SellerTerms> terms;
        for(auto c : selected)
            terms.push_back(c->announcedModeAndTermsFromPeer().sellModeTerms());

        std::vector<uint64_t> funds = distributeFunds(terms);

        assert(funds.size() == numberOfSellers);

        // Total amount that will be commited: that is not sent to change or tx fee
        uint64_t totalComitted = std::accumulate(funds.begin(), funds.end(), 0);

        /////////////////////////

        // Determine the contract fee
        uint64_t contractFeePerKb = 0;

        for(auto c : selected) {

            uint64_t minContractFeePerKb = c->announcedModeAndTermsFromPeer().sellModeTerms().minContractFeePerKb();

            // If this sellers has a greater minimal fee, then we must respect that
            if(minContractFeePerKb > contractFeePerKb)
                contractFeePerKb = minContractFeePerKb;
        }

        // Determine the change amount if any (!=0)
        uint64_t changeAmount = determineChangeAmount(numberOfSellers, totalComitted, contractFeePerKb, _funding.size());

        // Ensure Enough funds available to fund the contract
        uint64_t estimatedFee = paymentchannel::Contract::fee(numberOfSellers, true, contractFeePerKb, _funding.size());

        if(_funding.value() < (totalComitted + changeAmount + estimatedFee))
            throw std::runtime_error("Aborted trying to start download, not enough funding provided");

        // Create sellers
        for(auto c : selected)
            _sellers[c->connectionId()] = detail::Seller<ConnectionIdType>(SellerState::waiting_to_be_assigned_piece, c, 0);

        /////////////////////////

        // Create contract
        // Note: must be done before sending ready message on wire,
        // as it requires the contract txid, which is based on outputs
        paymentchannel::Contract c(_funding);

        // Generate keys and addresses required
        std::vector<Coin::PubKeyHash> finalPkHashes;

        for(const Coin::P2PKHAddress &finalAddress : _generateReceiveAddresses(numberOfSellers)){
            finalPkHashes.push_back(finalAddress.pubKeyHash());
        }

        // Generate contract key pairs
        std::vector<Coin::KeyPair> contractKeyPairs;
        std::vector<paymentchannel::Commitment> commitments;

        for(uint32_t i = 0; i < numberOfSellers; i++) {

            auto value = funds[i];
            auto payeeContractPk = selected[i]->payor().payeeContractPk();
            auto lockTime = terms[i].minLock();

            // Generate new buyer keypair for commitment
            Coin::KeyPair payorCommitmentKeyPair = _generateP2SHKeyPair([payeeContractPk, lockTime](const Coin::PublicKey & payorCommitmentPk){

                paymentchannel::RedeemScript redeemScript(payorCommitmentPk, payeeContractPk, lockTime);
                return redeemScript.serialized();

            }, paymentchannel::RedeemScript::PayorOptionalData());

            paymentchannel::Commitment commitment(value,
                                                  payorCommitmentKeyPair.pk(),
                                                  payeeContractPk,
                                                  lockTime);
            commitments.push_back(commitment);

            contractKeyPairs.push_back(payorCommitmentKeyPair);
        }

        assert(contractKeyPairs.size() == numberOfSellers);
        assert(commitments.size() == numberOfSellers);

        for(auto &commitment : commitments) {
            // Add commitment to contract
            c.addCommitment(commitment);
        }

        // Add change if worth doing
        if(changeAmount != 0) {

            // New change address
            Coin::P2PKHAddress address = _generateChangeAddresses(1).front();

            // Create and set change payment
            c.setChange(Coin::Payment(changeAmount, address));
        }

        // Create and store contract transaction
        _contractTx = c.transaction();

        // Make sure we are sending the right amount
        assert(_contractTx.getTotalSent() == totalComitted + changeAmount);

        // Make sure we are covering the intended fee rate
        uint64_t fee = _funding.value() - _contractTx.getTotalSent();
        float contractSizeKb = ((float)_contractTx.getSize())/1024;
        assert((float)fee >= (contractFeePerKb * contractSizeKb));

        // Notify client that transaction should be broadcasted
        _broadcastTransaction(_contractTx);

        /////////////////////////

        // Notify seller peers about contract being ready and broadcasted
        Coin::TransactionId txId(Coin::TransactionId::fromTx(_contractTx));

        for(uint32_t i = 0;i < numberOfSellers;i++)
            selected[i]->processEvent(protocol_statemachine::event::ContractPrepared(Coin::typesafeOutPoint(txId, i),
                                                                                     contractKeyPairs[i],
                                                                                     finalPkHashes[i],
                                                                                     funds[i]));

        /////////////////////////

        // Update state of sessions,
        // has to be done before starting to assign pieces to sellers
        _state = BuyingState::downloading;

        /////////////////////////

        // Assing first piece to each seller
        for(auto & mapping : _sellers) {

            // Assign the first piece to this peer
            bool assigned = tryToAssignAndRequestPiece(_sellers[mapping.first]);

            // Unless there are more peers than unassigned pieces
            // this should always work
            assert(assigned);
        }

        /////////////////////////

        std::cout << "Started downloading." << std::endl;

        return true;
    }

    template <class ConnectionIdType>
    std::vector<detail::Connection<ConnectionIdType> *> Buying<ConnectionIdType>::selectSellers() const {

        // Find peers that have joined our last invitation.
        // This implicitly guarantees that it is in sell mode, and satisfies our terms
        std::vector<detail::Connection<ConnectionIdType> *> joinedSellers = _session-> template connectionsInState<protocol_statemachine::PreparingContract>();

        // Pick how many sellers we actualy want.
        // NB**: in future, we base it on policy,
        // and that will also influence how we select sellers,
        // as they have constraints on this.
        uint N = _terms.minNumberOfSellers();

        // Stop if there are fewer than the absolute minimum
        if(joinedSellers.size() < N)
            return std::vector<detail::Connection<ConnectionIdType> *>();
        else if((joinedSellers.size() > N)) {

            // If we have more than desired number, we rank them accoring to terms
            // NB**: in future, base it on policy
            std::sort(joinedSellers.begin(), joinedSellers.end(),
                      [this](detail::Connection<ConnectionIdType> * a, detail::Connection<ConnectionIdType> * b) -> bool {
                      return protocol_wire::SellerTerms::compare(this->_policy.sellerTermsOrderingPolicy(),
                                                                 a->announcedModeAndTermsFromPeer().sellModeTerms(),
                                                                 b->announcedModeAndTermsFromPeer().sellModeTerms());
            });

            // Only keep top N
            joinedSellers.resize(N);
        }

        assert(joinedSellers.size() == N);

        return joinedSellers;
    }

    template <class ConnectionIdType>
    std::vector<uint64_t> Buying<ConnectionIdType>::distributeFunds(const std::vector<protocol_wire::SellerTerms> & terms) const {

        //NB: Based on policy in the future
        std::vector<uint64_t> funds;
        for(protocol_wire::SellerTerms t : terms)
            funds.push_back(t.minPrice() * _pieces.size());

        return funds;
    }

    template <class ConnectionIdType>
    uint64_t Buying<ConnectionIdType>::determineChangeAmount(uint32_t numberOfSellers, uint64_t totalComitted, uint64_t contractFeePerKb, int numberOfInputs) const {

        // Contract fee when there is a change output
        uint64_t contractTxFeeWithChangeOutput = paymentchannel::Contract::fee(numberOfSellers, true, contractFeePerKb, numberOfInputs);

        // Amount to use as change, if we are going to have change
        uint64_t potentialChangeAmount = _funding.value() - totalComitted - contractTxFeeWithChangeOutput;

        // Return potential change amount if it exeeds dust limit
        if(potentialChangeAmount > BITCOIN_DUST_LIMIT)
            return potentialChangeAmount;
        else
            return 0;
    }

    template <class ConnectionIdType>
    bool Buying<ConnectionIdType>::tryToAssignAndRequestPiece(detail::Seller<ConnectionIdType> & s) {

        assert(_session->_state == SessionState::started);
        assert(_state == BuyingState::downloading);
        assert(s.state() == SellerState::waiting_to_be_assigned_piece);

        // Try to find index of next unassigned piece
        int pieceIndex;

        // Prioritize deassigned piece
        if(!_deAssignedPieces.empty()) {
            pieceIndex = _deAssignedPieces.front();
            _deAssignedPieces.pop_front();
        } else {

            try {
                pieceIndex = getNextUnassignedPiece();
            } catch(const std::runtime_error & e) {
                // No unassigned piece was found
                return false;
            }
        }

        // Assign piece to seller
        _pieces[pieceIndex].assigned(s.connection()->connectionId());

        // Request piece from seller
        s.requestPiece(pieceIndex);

        // Update assignment bound
        _assignmentLowerBound = pieceIndex + 1;

        return true;
    }

    template <class ConnectionIdType>
    int Buying<ConnectionIdType>::getNextUnassignedPiece() const {

         // Look in interval [_assignmentLowerBound, end]
         for(uint32_t i = _assignmentLowerBound;i < _pieces.size();i++)
             if(_pieces[i].state() == PieceState::unassigned)
                 return i;

         // Then try interval [0, _assignmentLowerBound)
         for(uint32_t i = 0;i < _assignmentLowerBound;i++)
             if(_pieces[i].state() == PieceState::unassigned)
                 return i;

         // We did not find anything
         throw std::runtime_error("Unable to find any unassigned pieces.");
    }

    template<class ConnectionIdType>
    typename detail::ConnectionMap<ConnectionIdType>::const_iterator Buying<ConnectionIdType>::removeConnection(const ConnectionIdType & id, DisconnectCause cause) {

        assert(_session->state() != SessionState::stopped);
        assert(_session->hasConnection(id));

        // If this is the connection of a seller,
        // we have to deal with that.
        auto itr = _sellers.find(id);

        if(itr != _sellers.cend()) {

            detail::Seller<ConnectionIdType> & s = itr->second;

            // Seller can't be gone
            assert(s.state() != SellerState::gone);

            // Remove
            removeSeller(s);
        }

        // Destroy connection - important todo before notifying client
        auto it = _session->destroyConnection(id);

        // Notify client to remove connection
        _removedConnection(id, cause);

        return it;
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::removeSeller(detail::Seller<ConnectionIdType> & s) {

        // If this seller is assigned a piece, then we must unassign it
        if(s.state() == SellerState::waiting_for_full_piece ||
           s.state() == SellerState::waiting_for_piece_validation_and_storage) {

            // we must be downloading then
            assert(_state == BuyingState::downloading);

            // Get piece
            detail::Piece<ConnectionIdType> & piece = _pieces[s.indexOfAssignedPiece()];

            // Check that it hasnt already been downloaded out of bounds
            if(piece.state() != PieceState::downloaded) {

                // waiting_for_full_piece -> being_downloaded
                assert(s.state() != SellerState::waiting_for_full_piece ||
                       piece.state() == PieceState::being_downloaded);

                // waiting_for_piece_validation_and_storage -> being_validated_and_stored
                assert(s.state() != SellerState::waiting_for_piece_validation_and_storage ||
                        piece.state() == PieceState::being_validated_and_stored);

                // Mark as not assigned
                piece.deAssign();

                // Add to queue of unassigned pieces
                _deAssignedPieces.push_back(piece.index());
            }
        }

        // Mark as seller as gone
        s.removed();
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::politeSellerCompensation() {

        // Pay any seller we have requested a piece from which
        // has not been shown to be invalid, or even arrived.
        // NB: paying for only requested piece can lead to our payment
        // being dropped by peer state machine if it has not yet sent
        // the piece, but its worth trying.
        for(auto itr : _sellers) {

            detail::Seller<ConnectionIdType> & s = itr.second;

            if(s.isPossiblyOwedPayment())
                s.pieceWasValid();
        }
    }

    template <class ConnectionIdType>
    BuyingPolicy Buying<ConnectionIdType>::policy() const {
        return _policy;
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::setPolicy(const BuyingPolicy & policy) {
        _policy = policy;
    }

    template <class ConnectionIdType>
    Coin::UnspentOutputSet Buying<ConnectionIdType>::funding() const {
        return _funding;
    }

    template <class ConnectionIdType>
    protocol_wire::BuyerTerms Buying<ConnectionIdType>::terms() const {
        return _terms;
    }

}
}
}
