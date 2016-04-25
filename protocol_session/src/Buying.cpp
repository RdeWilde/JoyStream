/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#include <protocol_session/Buying.hpp>
#include <protocol_session/TorrentPieceInformation.hpp>
#include <protocol_session/Exceptions.hpp>
#include <common/Bitcoin.hpp> // BITCOIN_DUST_LIMIT
#include <common/P2PKHAddress.hpp>

#include <algorithm>

namespace joystream {
namespace protocol_session {

    template <class ConnectionIdType>
    Buying<ConnectionIdType>::Buying(const RemovedConnectionCallbackHandler<ConnectionIdType> & removedConnectionCallbackHandler,
                                     const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                     const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                                     const BroadcastTransaction & broadcastTransaction,
                                     const FullPieceArrived<ConnectionIdType> & fullPieceArrived,
                                     const Coin::UnspentP2PKHOutput & funding,
                                     const Policy & policy,
                                     const protocol_wire::BuyerTerms & terms,
                                     const TorrentPieceInformation & information)
        : _sessionCore(removedConnectionCallbackHandler,
                       generateKeyPairsCallbackHandler,
                       generateP2PKHAddressesCallbackHandler)
        , _fullPieceArrived(fullPieceArrived)
        , _funding(funding)
        , _policy(policy)
        , _terms(terms)
        , _assignmentLowerBound(0)
        , _lastStartOfSendingInvitations(0) {

        // Note starting time
        //time(&_lastStart);

        // Setup pieces
        for(const PieceInformation & p : information.pieces())
            _pieces.push_back(detail::Piece<ConnectionIdType>(p));
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::tick() {

        // Only process if we are active
        if(_state._clientState == State::ClientState::started) {

            if(_state._startedState == State::StartedState::sending_invitations)
                tryToStartDownloading();
            else if(_state._startedState == State::StartedState::downloading)
                unAssignPiecesFromSlowSellers();
        }
    }

    template <class ConnectionIdType>
    uint Buying<ConnectionIdType>::addConnection(const ConnectionIdType & id, const SendMessageOnConnection & callback) {

        // Do not accept new connection
        if(_state._clientState == State::ClientState::stopped)
            throw exception::StateIncompatibleOperation();

        // Check that connection is new, throw exception if not
        if(_sessionCore.hasConnection(id))
            throw exception::ConnectionAlreadyAddedException<ConnectionIdType>(id);

        // Create a new connection
        detail::Connection<ConnectionIdType> * connection = new detail::Connection<ConnectionIdType>(id);

        // Set message sending callback
        connection->machine().setSendMessage(callback);

        // Setup hooks into *this
        setHooks(connection);

        // Choose the mode of the connection
        connection->_machine.process_event(protocol_statemachine::event::BuyModeStarted(_terms));

        // Add to map
        _sessionCore._connections[id] = connection;

        return _sessionCore._connections.size();
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::removeConnection(const ConnectionIdType & id) {

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::processMessageOnConnection(const ConnectionIdType & id, const protocol_wire::ExtendedMessagePayload & m) {

        // No messages should ever arrive when we have stopped,
        // given that connections should have been closed
        if(_state._clientState == State::ClientState::stopped)
            throw exception::StateIncompatibleOperation();

        // We have to accept, at least one message, which may be in
        // transit when we initiate pause, and also just mode updates.

        // Get connection
        detail::Connection<ConnectionIdType> * c = _sessionCore.get(id);

        // and, have it process the message
        c->processMessage(m);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::validPieceReceivedOnConnection(const ConnectionIdType &, int index) {

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::invalidPieceReceivedOnConnection(const ConnectionIdType &, int index) {

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::updateTerms(const protocol_wire::BuyerTerms & terms) {

        // you are invalidating a bunch of invites: <--- state machine mayhandle
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::toObserveMode() {

    }

    template <class ConnectionIdType>
    Selling<ConnectionIdType> * Buying<ConnectionIdType>::toSellMode() {

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::start() {

        // Note starting time
        time(&_lastStartOfSendingInvitations);

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::stop() {


        // only set as stopped when  all peer have been disconnected,
        // its up to client to never send us a new message if there is some
        // delay in actually shutting down the connection
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::pause() {

        // move ut to states on connections where
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::setHooks(detail::Connection<ConnectionIdType> & connection) {

        protocol_statemachine::CBStateMachine & machine = connection.machine();
        ConnectionIdType id = connection.connectionId();

        machine.setPeerAnnouncedMode([this, &id](const protocol_statemachine::AnnouncedModeAndTerms & a) {
            this->peerAnnouncedModeAndTerms(id, a);
        });

        machine.setInvitedToOutdatedContract([this, &id](void) {
            this->invitedToOutdatedContract(id);
        });

        machine.setInvitedToJoinContract([this, &id]() {
            this->invitedToJoinContract(id);
        });

        machine.setContractIsReady([this, &id](const Coin::typesafeOutPoint & o) {
            this->contractPrepared(id, o);
        });

        machine.setPieceRequested([this, &id](int i) {
            this->pieceRequested(id, i);
        });

        machine.setInvalidPieceRequested([this, &id]() {
            this->invalidPieceRequested(id);
        });

        machine.setPeerInterruptedPayment([this, &id]() {
            this->paymentInterrupted(id);
        });

        machine.setValidPayment([this, &id](const Coin::Signature & s) {
            this->receivedValidPayment(id, s);
        });

        machine.setInvalidPayment([this, &id](const Coin::Signature & s) {
            this->receivedInvalidPayment(id, s);
        });

        machine.setSellerJoined([this, &id]() {
            this->sellerHasJoined(id);
        });

        machine.setSellerInterruptedContract([this, &id]() {
            this->sellerHasInterruptedContract(id);
        });

        machine.setReceivedFullPiece([this, &id](const protocol_wire::PieceData & p) {
            this->receivedFullPiece(id, p);
        });
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::peerAnnouncedModeAndTerms(const ConnectionIdType & id, const protocol_statemachine::AnnouncedModeAndTerms & a) {

        // If we are currently active and sending out invitations, then we may (re)invite
        // sellers with sufficiently good terms
        if(_state._clientState == State::ClientState::active &&
           _state._startedState == State::StartedState::sending_invitations) {

            // Check that this peer is seller,
            protocol_statemachine::ModeAnnounced m = a.modeAnnounced();
            assert(m != protocol_statemachine::ModeAnnounced::none);

            // and has good enough terms to warrant an invitation
            if(m == protocol_statemachine::ModeAnnounced::sell && _terms.satisfiedBy(a.sellModeTerms())) {

                assert(_sessionCore->hasConnection(id));

                // Send invitation
                detail::Connection<ConnectionIdType> * c = _sessionCore->_connections.find(id)->second;

                c->_machine.process_event(protocol_statemachine::event::InviteSeller());

                std::cout << "Invited: " << toString(id);
            }

        }
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::invitedToOutdatedContract(const ConnectionIdType &) {

        // Should never happen, is only valid when selling
        assert(false);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::invitedToJoinContract(const ConnectionIdType &) {

        // Should never happen, is only valid when selling
        assert(false);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::contractPrepared(const ConnectionIdType &, const Coin::typesafeOutPoint &) {

        // Should never happen, is only valid when selling
        assert(false);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::pieceRequested(const ConnectionIdType & id, int i) {

        // Should never happen, is only valid when selling
        assert(false);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::invalidPieceRequested(const ConnectionIdType & id) {

        // Should never happen, is only valid when selling
        assert(false);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::paymentInterrupted(const ConnectionIdType & id) {

        // Should never happen, is only valid when selling
        assert(false);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::receivedValidPayment(const ConnectionIdType & id, const Coin::Signature &) {

        // Should never happen, is only valid when selling
        assert(false);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::receivedInvalidPayment(const ConnectionIdType & id, const Coin::Signature &) {

        // Should never happen, is only valid when selling
        assert(false);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::sellerHasJoined(const ConnectionIdType & id) {

        if(_state._clientState == State::ClientState::started &&
           _state._startedState == State::StartedState::sending_invitations)
            tryToStartDownloading();
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::sellerHasInterruptedContract(const ConnectionIdType & id) {

        // Get referemce to seller corresponding connection
        auto itr = _sellers.find(id);
        assert(itr != _sellers.cend());

        // Remove
        itr->second.removed();

        // Remove connection
        _sessionCore.remove(id);

        // Tell client to remove his pad peer
        _sessionCore._removedConnectionCallbackHandler(id, DisconnectCause::seller_has_interrupted_contract);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::receivedFullPiece(const ConnectionIdType & id, const protocol_wire::PieceData & p) {

        // Deal with other possible states we are in, e.g.
        // paused or whatever

        assert(_state._clientState == State::ClientState::started);
        assert(_state._startedState == State::StartedState::sending_invitations);

        // Get seller corresponding to given id
        auto itr = _sellers.find(id);
        assert(itr != _sellers.cend());

        detail::Seller<ConnectionIdType> & s = *itr;

        assert(s.state() == detail::Seller<ConnectionIdType>::State::waiting_for_full_piece);

        // Update seller state
        s.fullPieceArrived();

        //
        detail::Piece<ConnectionIdType> & piece = _pieces[s.indexOfAssignedPiece()];



        /***
        // update sellers state

        // ask client to validate

        // where do we mark that we are waiting for this callback?

        // if this was last downloaded piece, do we switch to inactive mode? or
        // how do we indicate that we in the future do not require trying
        // to search for unassigned pieces?


        // This routine should only be called if following holds
        Q_ASSERT(_state == BuyerTorrentPluginState::downloading_pieces);

        // Get what piece has been assigned to this peer
        int pieceIndex = peer->indexOfAssignedPiece();

        // Check that piece has correct length
        if(length != _torrent->torrent_file().piece_size(pieceIndex))
            throw std::runtime_error("Full piece message had invalid length.");

        // Tell libtorrent to validate piece
        // last argument is a flag which presently seems to only test
        // flags & torrent::overwrite_existing, which seems to be whether
        // the piece should be overwritten if it is already present
        //
        // libtorrent::torrent_plugin::on_piece_pass()
        // libtorrent::torrent_plugin::on_piece_failed()
        // processes result of checking
        _torrent->add_piece(pieceIndex, piece.get(), 0);

        // Update client state
        peer->setClientState(BuyerPeerPluginClientState::waiting_for_libtorrent_to_validate_piece);
        */


        // if we cant assign pieces, we need to say we are done
        // send signal?

    }

    template <class ConnectionIdType>
    bool Buying<ConnectionIdType>::tryToStartDownloading() {

        std::cout << "Trying to start downloading." << std::endl;

        assert(_state._clientState == State::ClientState::started);
        assert(_state._startedState == State::StartedState::sending_invitations);
        assert(_sellers.size() == 0);
        assert(_pieces.size() > 0);

        /////////////////////////

        // Does policy allow us to build contract at present time
        if(!_policy.okToBuildContract(_lastStartOfSendingInvitations))
            return false;

        // Select appropriate sellers
        std::vector<detail::Connection<ConnectionIdType> *> selected = selectSellers();

        uint32_t numberOfSellers = selected.size();

        /////////////////////////

        // Determine fund distribution among sellers
        std::vector<protocol_wire::SellerTerms> terms;
        for(const detail::Connection<ConnectionIdType> * c: selected)
            terms.push_back(c->seller->machine().announcedModeAndTermsFromPeer().sellModeTerms());

        std::vector<uint64_t> funds = distributeFunds(terms);

        assert(funds.size() == numberOfSellers);

        // Total amount that will be commited: that is not sent to change or tx fee
        uint64_t totalComitted = std::accumulate(funds.begin(), funds.end(), 0);

        /////////////////////////

        // Determine the contract fee
        uint64_t contractFeePerKb = 0;

        for(const detail::Connection<ConnectionIdType> * c : selected) {

            uint64_t minContractFeePerKb = c->machine().announcedModeAndTermsFromPeer().sellModeTerms().minContractFeePerKb();

            // If this sellers has a greater minimal fee, then we must respect that
            if(minContractFeePerKb > contractFeePerKb)
                contractFeePerKb = minContractFeePerKb;
        }

        // Determine the change amount if any (!=0)
        uint64_t changeAmount = determineChangeAmount(totalComitted, contractFeePerKb);

        /////////////////////////

        // Create contract
        // Note: must be done before sending ready message on wire,
        // as it requires the contract txid, which is based on outputs
        paymentchannel::Contract c;

        // Generate keys and addresses required
        std::vector<Coin::KeyPair> contractKeyPairs = _sessionCore->_generateKeyPairsCallbackHandler(numberOfSellers);
        std::vector<Coin::PubKeyHash> finalPkHashes = _sessionCore->_generateP2PKHAddressesCallbackHandler(numberOfSellers);

        // Create and add commitment to contract
        for(int i = 0;i < numberOfSellers;i++)
            c.addCommitment(paymentchannel::Commitment(funds[i],
                                                       contractKeyPairs[i].pk(),
                                                       selected[i]->machine().payor().payeeContractPk()));

        // Add change if worth doing
        if(changeAmount != 0) {

            // New change address
            Coin::P2PKHAddress address = _sessionCore->_generateP2PKHAddressesCallbackHandler(1);

            // Create and set change payment
            c.setChange(Coin::Payment(changeAmount, address.pubKeyHash()));
        }

        // Create and store contract transaction
        _contractTx = c.transaction();

        // Notify client that transaction should be broadcasted
        _broadcastTransaction(_contractTx);

        // Make sure we are sending the right amount
        assert(_contractTx.getTotalSent() == totalComitted + changeAmount);

        // Make sure we are covering the intended fee rate
        assert(((float)(_funding.value() - _contractTx.getTotalSent()))/_contractTx.getSize() >= contractFeePerKb);

        /////////////////////////

        // Notify seller peers about contract being ready and broadcasted
        Coin::TransactionId txId(Coin::TransactionId::fromTx(_contractTx));

        for(int i = 0;i < numberOfSellers;i++)
            selected[i]->processEvent(protocol_statemachine::event::ContractPrepared(Coin::typesafeOutPoint(txId, i), contractKeyPairs[i], finalPkHashes[i], funds[i]));

        /////////////////////////

        // Update state of sessions,
        // has to be done before starting to assign pieces to sellers
        _state._startedState = State::StartedState::downloading;

        /////////////////////////

        // Create sellers and add to seller mapping, and assign first piece
        for(const detail::Connection<ConnectionIdType> * c : selected) {

            // Create and store seller
            _sellers[c->connectionId()] = detail::Seller<ConnectionIdType>(detail::Seller<ConnectionIdType>::State::waiting_to_be_assigned_piece, c, 0);

            // Assign the first piece to this peer
            bool assigned = tryToAssignAndRequestPiece(_sellers[c->connectionId()]);

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
        std::vector<detail::Connection<ConnectionIdType> *> joinedSellers = _sessionCore-> template connectionsInState<protocol_statemachine::WaitingToStart>();

        // Pick how many sellers we actualy want.
        // NB**: in future, we base it on policy,
        // and that will also influence how we select sellers,
        // as they have constraints on this.
        uint N = _terms.minNumberOfSellers();

        // Stop if there are fewer than the absolute minimum
        if(joinedSellers.size() < N)
            return false;
        else if((joinedSellers.size() > N)) {

            // If we have more than desired number, we rank them accoring to terms
            // NB**: in future, base it on policy
            std::sort(joinedSellers.begin(), joinedSellers.end(),
                      [](const detail::Connection<ConnectionIdType> * a, const detail::Connection<ConnectionIdType> * b) -> bool {
                        return a->machine().announcedModeAndTermsFromPeer().sellModeTerms() > b->machine().announcedModeAndTermsFromPeer().sellModeTerms();
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
    uint64_t Buying<ConnectionIdType>::determineChangeAmount(uint32_t numberOfSellers, uint64_t totalComitted, uint64_t contractFeePerKb) const {

        // Contract fee when there is a change output
        uint64_t contractTxFeeWithChangeOutput = paymentchannel::Contract::fee(numberOfSellers, true, contractFeePerKb);

        // Whether a change output would be greater than dust
        bool changeWorthIncluding = _funding.value() - totalComitted - contractTxFeeWithChangeOutput > BITCOIN_DUST_LIMIT;

        //uint64_t contractTxFee;
        uint64_t changeAmount;

        if(changeWorthIncluding) {
            //contractTxFee = contractTxFeeWithChangeOutput;
            changeAmount = _funding.value() - totalComitted - contractTxFeeWithChangeOutput;
        } else {
            //contractTxFee = paymentchannel::Contract::fee(numberOfSellers, false, contractFeePerKb);
            changeAmount = 0;
        }

        return changeAmount;
    }

    template <class ConnectionIdType>
    bool Buying<ConnectionIdType>::tryToAssignAndRequestPiece(detail::Seller<ConnectionIdType> & s) {

        assert(_state._clientState == State::ClientState::started);
        assert(_state._startedState == State::StartedState::downloading);
        assert(s.state() == detail::Seller<ConnectionIdType>::State::waiting_to_be_assigned_piece);

        // Try to find index of next unassigned piece
        int pieceIndex;

        // Prioritize deassigned piece
        if(!_deAssignedPieces.empty())
            pieceIndex = _deAssignedPieces.pop();
        else {

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
         for(int i = _assignmentLowerBound;i < _pieces.size();i++)
             if(_pieces[i].state() == detail::Piece<ConnectionIdType>::State::unassigned)
                 return i;

         // Then try interval [0, _assignmentLowerBound)
         for(int i = 0;i < _assignmentLowerBound;i++)
             if(_pieces[i].state() == detail::Piece<ConnectionIdType>::State::unassigned)
                 return i;

         // We did not find anything
         throw std::runtime_error("Unable to find any unassigned pieces.");
    }

    template <class ConnectionIdType>
    uint Buying<ConnectionIdType>::unAssignPiecesFromSlowSellers() {

        // assert??

        for(detail::Seller<ConnectionIdType> & s: _sellers) {
            if(s.servicingPieceHasTimedOut(_policy._servicingPieceTimeOutLimit))
                ; /// MAKE SOME SORT CALLS!!!***
        }
    }

    template <class ConnectionIdType>
    typename Buying<ConnectionIdType>::Policy Buying<ConnectionIdType>::getPolicy() const {
        return _policy;
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::setPolicy(const Policy &policy) {
        _policy = policy;
    }

    template <class ConnectionIdType>
    Coin::UnspentP2PKHOutput Buying<ConnectionIdType>::getFunding() const {
        return _funding;
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::setFunding(const Coin::UnspentP2PKHOutput &funding) {
        _funding = funding;
    }
}
}
