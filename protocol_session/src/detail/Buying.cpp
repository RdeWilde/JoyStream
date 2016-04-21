/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#include <protocol_session/detail/Buying.hpp>
#include <protocol_session/detail/SessionCoreImpl.hpp>
#include <protocol_statemachine/protocol_statemachine.hpp>
#include <common/Bitcoin.hpp> // BITCOIN_DUST_LIMIT
#include <common/P2PKHAddress.hpp>

#include <algorithm>

namespace joystream {
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    Buying<ConnectionIdType>::Buying(detail::SessionCoreImpl<ConnectionIdType> * sessionCore)
        : _sessionCore(sessionCore)
        , _lastAssignedPiece(0)
        , _assignmentLowerBound(0)
        , _lastStart(0)
        , _lastStartOfSendingInvitations(0) {

        // Note starting time
        time(&_lastStart);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::tick() {

        // Only process if we are active
        if(_state._state == BuyingState::State::active) {

            if(_state._active == BuyingState::Active::sending_invitations)
                tryToStartDownloading();
            else {

                assert(_state._active == BuyingState::Active::downloading);

                // Check if there are any outstanding requests which are taking
                // too long, and therefore should be canceled

                // Are there any sellers which are waiting to be assigned a piece? can that happen

                //assignPiecesToIdleSellers();
            }
        }
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::connectionAdded(const ConnectionIdType & id) {

        // Get connection
        auto itr =_sessionCore->_connections.find(id);

        assert(itr != _sessionCore->_connections.end());

        detail::Connection<ConnectionIdType> * connection = itr->second;

        // Choose the mode of the connection
        connection->_machine.process_event(protocol_statemachine::event::BuyModeStarted(_terms));
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::connectionRemoved(const ConnectionIdType & id) {

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::to(BuyingState::State state) {

        // updat _lastStartOfSendingInvitations

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::updateTerms(const protocol_wire::BuyerTerms & terms) {

        // you are invalidating a bunch of invites: <--- state machine mayhandle
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::peerAnnouncedModeAndTerms(const ConnectionIdType & id, const protocol_statemachine::AnnouncedModeAndTerms & a) {

        // If we are currently active and sending out invitations, then we may (re)invite
        // sellers with sufficiently good terms
        if(_state._state == BuyingState::State::active &&
           _state._active == BuyingState::Active::sending_invitations) {

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
    void Buying<ConnectionIdType>::sellerHasJoined(const ConnectionIdType & id) {

        if(_state._state == BuyingState::State::active &&
           _state._active == BuyingState::Active::sending_invitations)
            tryToStartDownloading();
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::sellerHasInterruptedContract(const ConnectionIdType & id) {

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::receivedFullPiece(const ConnectionIdType & id, const protocol_wire::PieceData & p) {

        // update sellers state

        // ask client to validate

        // where do we mark that we are waiting for this callback?

        // if this was last downloaded piece, do we switch to inactive mode? or
        // how do we indicate that we in the future do not require trying
        // to search for unassigned pieces?

        /**
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
    }

    template <class ConnectionIdType>
    bool Buying<ConnectionIdType>::tryToStartDownloading() {

        std::cout << "Trying to start downloading." << std::endl;

        assert(_state._state == BuyingState::State::active);
        assert(_state._active == BuyingState::Active::sending_invitations);
        assert(_sellers.size() == 0);
        assert(_pieces.size() > 0);

        /////////////////////////

        // Does policy allow us to build contract at present time
        if(!_policy.okToBuildContract(_lastStartOfSendingInvitations))
            return false;

        // Find peers that have joined our last invitation.
        // This implicitly guarantees that it is in sell mode, and satisfies our terms
        std::vector<Connection<ConnectionIdType> *> joinedSellers = _sessionCore-> template connectionsInState<protocol_statemachine::WaitingToStart>();

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
                      [](const Connection<ConnectionIdType> * a, const Connection<ConnectionIdType> * b) -> bool {
                        return a->machine().announcedModeAndTermsFromPeer().sellModeTerms() > b->machine().announcedModeAndTermsFromPeer().sellModeTerms();
            });

        }

        // Return top N
        std::vector<Connection<ConnectionIdType> *> selectedSellers;
        for(int i = 0;i < N;i++)
            selectedSellers.push_back(joinedSellers[i]);

        assert(selectedSellers.size() == N);

        /////////////////////////

        // Determine fee and distribution in contract transaction
        uint64_t contractFeePerKb = 0;
        uint64_t totalFundsToCommit = 0;
        std::vector<uint64_t> commitFunds;

        for(const Connection<ConnectionIdType> * c : selectedSellers) {

            // Get terms
            protocol_wire::SellerTerms t = c->machine().announcedModeAndTermsFromPeer().sellModeTerms();

            // Contract fee
            int64_t fee = t.minContractFeePerKb();

            if(fee > contractFeePerKb)
                contractFeePerKb = fee;

            // Commitment funds
            int64_t funds = t.minPrice()*_pieces.size();

            totalFundsToCommit += funds;
            commitFunds.push_back(funds);
        }

        assert(commitFunds.size() == N);

        // Contract fee when there is a change output
        uint64_t contractTxFeeWithChangeOutput = paymentchannel::Contract::fee(N, true, contractFeePerKb);

        // Whether a change output would be greater than dust
        bool changeWorthIncluding = _funding.value() - totalFundsToCommit - contractTxFeeWithChangeOutput > BITCOIN_DUST_LIMIT;

        uint64_t contractTxFee;
        uint64_t changeAmount;

        if(changeWorthIncluding) {
            contractTxFee = contractTxFeeWithChangeOutput;
            changeAmount = _funding.value() - totalFundsToCommit - contractTxFee;
        } else {
            contractTxFee = paymentchannel::Contract::fee(N, false, contractFeePerKb);
            changeAmount = 0;
        }

        /**
        // Determine how to distribute funds across sellers
        std::vector<protocol_wire::SellerTerms> terms;
        for(const Connection<ConnectionIdType> * c : selectedSellers)
            terms.push_back(c->machine().announcedModeAndTermsFromPeer().sellModeTerms());

        assert(terms.size() == N);

        std::vector<int64_t> distribution = distributeFunds(_funding.value(), contractTxFee, terms, _pieces.size());
        */

        /////////////////////////

        // Create contract
        // Note: must be done before sending ready message on wire,
        // as it requires the contract txid, which is based on outputs
        paymentchannel::Contract c;

        // Collect private keys for payor side of contract
        std::vector<Coin::KeyPair> contractKeyPairs;

        for(int i = 0;i < N;i++) {

            // Generate a contract key pair
            Coin::KeyPair contractKeyPair = _sessionCore->_generateKeyPairsCallbackHandler(1);

            // Store private key for later when issuing seller state machine events about contract being ready
            contractKeyPairs.push_back(contractKeyPair);

            // Add commitment to contract
            c.addCommitment(paymentchannel::Commitment(commitFunds[i],
                                                       contractKeyPair.pk(),
                                                       selectedSellers[i]->machine().payor().payeeContractPk()));
                                                        // can be simplified if we go through seller, and it has pointer to connection
        }

        // Add change if worth doing
        if(changeWorthIncluding) {

            // New change address
            Coin::P2PKHAddress address = _sessionCore->_generateP2PKHAddressesCallbackHandler(1);

            // Create and set change payment
            c.setChange(Coin::Payment(changeAmount, address.pubKeyHash()));
        }

        // Create and store contract transaction
        _contractTx = c.transaction();

        // Notify client that transaction should be broadcasted
        _sessionCore->broadcastContract(_contractTx);

        // Make sure we are sending the right amount
        assert(_contractTx.getTotalSent() == totalFundsToCommit + changeAmount);

        // Make sure we are covering the intended fee rate
        assert(((float)(_funding.value() - _contractTx.getTotalSent()))/_contractTx.getSize() >= contractFeePerKb);

        /////////////////////////

        // Create sellers
        Coin::TransactionId txId(Coin::TransactionId::fromTx(_contractTx));

        for(int i = 0;i < N;i++) {

            Connection<ConnectionIdType> * c = selectedSellers[i];

            assert(c._downloadedValidPieces.size() == 0);

            // Create seller
            Seller<ConnectionIdType> s(Seller<ConnectionIdType>::State::waiting_to_be_assigned_piece, c->connectionId(), 0);

            // add to seller list
            _sellers.push_back(s);

            // Announce ready contract to sellers
            Coin::typesafeOutPoint anchor(txId, i);  // i is loose here, should be more rigid
            Coin::KeyPair contractKeyPair = contractKeyPairs[i];
            Coin::PubKeyHash finalPkHash = _sessionCore->_generateP2PKHAddressesCallbackHandler(1).pubKeyHash();
            uint64_t value = commitFunds[i];

            c.processEvent(protocol_statemachine::event::ContractPrepared(anchor, contractKeyPair, finalPkHash, value));

            // Assign the first piece to this peer
            //assignPieceToSeller(i);
        }

        std::vector<protocol_statemachine::event::ContractPrepared> _hello;

        /////////////////////////

        // Update state of sessions
        _state._active = BuyingState::Active::downloading;

        std::cout << "Started downloading." << std::endl;

        return true;
    }

    template <class ConnectionIdType>
    std::vector<int64_t> Buying<ConnectionIdType>::distributeFunds(int64_t funding, int64_t txFee, const std::vector<protocol_wire::SellerTerms> & selectedTerms, unsigned int numberOfPieces) {

        int64_t totalComitted = funding - txFee;

        // policy for how to distribute funds among channels?, based on policy

        // use price information in in seller terms
        // how much to refund,
        // how much to lockup
        // how to distribute across these

        // result
        return std::vector<int64_t>();
    }

    template <class ConnectionIdType>
    int Buying<ConnectionIdType>::tryToAssignAndRequestPiece(Seller<ConnectionIdType> & s) {

        cassert(s.state() == Seller<ConnectionIdType>::State::waiting_to_be_assigned_piece);

        return 0;
    }

    template <class ConnectionIdType>
    typename Buying<ConnectionIdType>::Policy Buying<ConnectionIdType>::getPolicy() const
    {
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
}
