/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#include <protocol_session/Session.hpp>
#include <protocol_session/Exceptions.hpp>
#include <protocol_session/detail/Selling.hpp>
#include <protocol_session/detail/Buying.hpp>
#include <protocol_session/detail/Observing.hpp>

namespace joystream {
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    Selling<ConnectionIdType>::Selling(Session<ConnectionIdType> * session,
                                       const RemovedConnectionCallbackHandler<ConnectionIdType> & removedConnection,
                                       const GenerateKeyPairsCallbackHandler & generateKeyPairs,
                                       const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddresses,
                                       const LoadPieceForBuyer<ConnectionIdType> & loadPieceForBuyer,
                                       const ClaimLastPayment<ConnectionIdType> & claimLastPayment,
                                       const AnchorAnnounced<ConnectionIdType> & anchorAnnounced,
                                       const SellingPolicy & policy,
                                       const protocol_wire::SellerTerms & terms)
        : _session(session)
        , _removedConnection(removedConnection)
        , _generateKeyPairs(generateKeyPairs)
        , _generateP2PKHAddresses(generateP2PKHAddresses)
        , _loadPieceForBuyer(loadPieceForBuyer)
        , _claimLastPayment(claimLastPayment)
        , _anchorAnnounced(anchorAnnounced)
        , _policy(policy)
        , _terms(terms) {

        // Notify any existing peers
        for(auto itr : _session->_connections)
            (itr.second)->processEvent(joystream::protocol_statemachine::event::SellModeStarted(_terms));
    }

    template<class ConnectionIdType>
    uint Selling<ConnectionIdType>::addConnection(const ConnectionIdType & id, const SendMessageOnConnection & callback) {

        // Create connection
        detail::Connection<ConnectionIdType> * connection = _session->createAndAddConnection(id, callback);

        // Choose mode on connection
        connection->processEvent(protocol_statemachine::event::SellModeStarted(_terms));

        return _session->_connections.size();
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::removeConnection(const ConnectionIdType & id) {

        if(_session->state() == SessionState::stopped)
            throw exception::StateIncompatibleOperation();

        if(!_session->hasConnection(id))
            throw exception::ConnectionDoesNotExist<ConnectionIdType>(id);

        // Remove connection
        removeConnection(id, DisconnectCause::client);
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::pieceLoaded(const ConnectionIdType & id, const protocol_wire::PieceData & data, int) {

        // We cannot have connection and be stopped
        assert(_session->state() != SessionState::stopped);

        if(!_session->hasConnection(id))
            throw exception::ConnectionDoesNotExist<ConnectionIdType>(id);

        // Get connection state
        detail::Connection<ConnectionIdType> * c = _session->get(id);

        if(!c-> template inState<joystream::protocol_statemachine::LoadingPiece>())
            throw exception::StateIncompatibleOperation();

        // Store loaded piece in connection, we dont sent if paused
        assert(!c->loadedPiecePending());
        c->setLoadedPiecePending(true);
        c->setLoadedPieceData(data);

        // If we are starte, then send off
        if(_session->state() == SessionState::started)
            tryToLoadPiece(c);
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::peerAnnouncedModeAndTerms(const ConnectionIdType & id, const protocol_statemachine::AnnouncedModeAndTerms &) {

        // We cannot have connection and be stopped
        assert(_session->state() != SessionState::stopped);

        // Connection must be live
        assert(_session->hasConnection(id));
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::invitedToOutdatedContract(const ConnectionIdType & id) {

        // We cannot have connection and be stopped
        assert(_session->state() != SessionState::stopped);

        // Connection must be live
        assert(_session->hasConnection(id));

        // Nothing to do
        // NB** in the future we may want to keep track of how often this happens
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::invitedToJoinContract(const ConnectionIdType & id) {

        // We cannot have connection and be stopped
        assert(_session->state() != SessionState::stopped);

        // Connection must be live
        assert(_session->hasConnection(id));

        // Do not initiate joining if we are paused
        if(_session->state() == SessionState::started) {


            detail::Connection<ConnectionIdType> * c = _session->get(id);

            // NB** In the future we do something more sophisticated rather than just joining right away?
            tryToJoin(c);
        }
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::contractPrepared(const ConnectionIdType & id, quint64 value, const Coin::typesafeOutPoint & anchor, const Coin::PublicKey & payorContractPk, const Coin::PubKeyHash & payorFinalPkHash) {

        // We cannot have connection and be stopped
        assert(_session->state() != SessionState::stopped);

        // Connection must be live
        assert(_session->hasConnection(id));

        // Notify client
        // NB** We do this, even if we are paused!
        _anchorAnnounced(id, value, anchor, payorContractPk, payorFinalPkHash);
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::pieceRequested(const ConnectionIdType & id, int index) {

        // We cannot have connection and be stopped
        assert(_session->state() != SessionState::stopped);

        // Connection must be live
        assert(_session->hasConnection(id));

        // Notify client about piece request
        // NB** We do this, even if we are paused!
        _loadPieceForBuyer(id, index);
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::invalidPieceRequested(const ConnectionIdType & id) {

        // We cannot have connection and be stopped
        assert(_session->state() != SessionState::stopped);

        // Connection must be live
        assert(_session->hasConnection(id));

        // Remove connection
        removeConnection(id, DisconnectCause::buyer_requested_invalid_piece);
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::paymentInterrupted(const ConnectionIdType & id) {

        // We cannot have connection and be stopped
        assert(_session->state() != SessionState::stopped);

        // Connection must be live
        assert(_session->hasConnection(id));

        // Remove connection
        removeConnection(id, DisconnectCause::buyer_interrupted_payment);
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::receivedValidPayment(const ConnectionIdType &, const Coin::Signature &) {
        // Nothing to do here, just good news really!
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::receivedInvalidPayment(const ConnectionIdType & id, const Coin::Signature &) {

        // We cannot have connection and be stopped
        assert(_session->state() != SessionState::stopped);

        // Connection must be live
        assert(_session->hasConnection(id));

        // Remove connection
        removeConnection(id, DisconnectCause::buyer_sent_invalid_payment);
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::leavingState() {

        //// Mode change is allowed in all session states

        // For each connection: Notify client to claim last payment made
        for(auto itr : _session->_connections) {

            detail::Connection<ConnectionIdType> * c = itr.second;

            if(c-> template inState<joystream::protocol_statemachine::WaitingForPayment>())
                _claimLastPayment(itr.first, c->machine().payee());
        }
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::start() {

        // We cant start if we have already started
        if(_session->state() == SessionState::started)
            throw exception::StateIncompatibleOperation();

        // Set client mode to started
        // NB: Mark as started, as routines we call below
        // require that we are started.
        _session->_state = SessionState::started;

        // For each connection
        for(auto itr : _session->_connections) {

            //// if we hare here, we are paused

            detail::Connection<ConnectionIdType> * c = itr.second;

            if(c-> template inState<protocol_statemachine::ReadyForInvitation>())
                ; // Waiting to be invited, nothing to do!
            else if(c-> template inState<protocol_statemachine::Invited>())
                tryToJoin(c); // We have been invited, so lets try to join
            else if(c-> template inState<protocol_statemachine::WaitingToStart>()) {
                ; // Waiting to get contract announcement, nothing to do!
            } else if(c-> template inState<protocol_statemachine::ReadyForPieceRequest>()) {
                ; // Waiting for piece to be requested, nothing to do!
            } else if(c-> template inState<protocol_statemachine::LoadingPiece>())
                tryToLoadPiece(c); // Waiting for piece to be loaded, which may have been aborted due to pause
            else if(c-> template inState<protocol_statemachine::WaitingForPayment>()) {
                ; // Waiting for payment, nothing to do!
            } else // terminated, should not happen
                assert(false);
        }
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::stop() {

        // We cant stop if we have already stopped
        if(_session->state() == SessionState::stopped)
            throw exception::StateIncompatibleOperation();

        // Disconnect everyone
        std::vector<ConnectionIdType> ids = _session->connectionIds();

        for(const ConnectionIdType & id : ids)
            removeConnection(id, DisconnectCause::client);

        // Update state
        _session->_state = SessionState::stopped;
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::pause() {

        // We can only pause if presently started
        if(_session->state() == SessionState::paused ||
           _session->state() == SessionState::stopped)
            throw exception::StateIncompatibleOperation();

        // Update state
        _session->_state = SessionState::paused;
    }

    template <class ConnectionIdType>
    void Selling<ConnectionIdType>::tick() {
        // Nothing to do
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::updateTerms(const protocol_wire::SellerTerms & terms) {

        // Notify existing peers where we have not yet joined the contract
        for(auto itr : _session->_connections) {

            detail::Connection<ConnectionIdType> * c = itr.second;

            if(c-> template inState<joystream::protocol_statemachine::ReadyForInvitation>() ||
               c-> template inState<joystream::protocol_statemachine::Invited>())
                c->processEvent(protocol_statemachine::event::UpdateTerms<protocol_wire::SellerTerms>(terms));
        }

        // Set new terms
        _terms = terms;
    }

    template<class ConnectionIdType>
    status::Selling Selling<ConnectionIdType>::status() const {
        return status::Selling(_policy, _terms);
    }

    template<class ConnectionIdType>
    protocol_wire::SellerTerms Selling<ConnectionIdType>::terms() const {
        return _terms;
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::removeConnection(const ConnectionIdType & id, DisconnectCause cause) {

        assert(_session->state() != SessionState::stopped);
        assert(_session->hasConnection(id));

        // Notify client to claim last payment
        detail::Connection<ConnectionIdType> * c = _session->get(id);

        // If we are owed money, then notify client to claim last payment made
        if(c-> template inState<joystream::protocol_statemachine::WaitingForPayment>())
            _claimLastPayment(id, c->machine().payee());

        // Notify client to remove connection
        _removedConnection(id, cause);

        // Remove and delete connection
        _session->removeFromMapAndDelete(id);
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::tryToJoin(detail::Connection<ConnectionIdType> * c) {

        assert(_session->state() == SessionState::started);
        assert(c-> template inState<joystream::protocol_statemachine::Invited>());

        // Figure out  if buyer terms are good enough
        protocol_statemachine::AnnouncedModeAndTerms announced = c->announcedModeAndTermsFromPeer();

        assert(announced.modeAnnounced() == protocol_statemachine::ModeAnnounced::buy);

        if(_terms.satisfiedBy(announced.buyModeTerms())) {

            // Join if they are
            Coin::KeyPair contractKeyPair = _generateKeyPairs(1).front();
            Coin::PubKeyHash finalPkHash = _generateP2PKHAddresses(1).front().pubKeyHash();
            c->processEvent(joystream::protocol_statemachine::event::Joined(contractKeyPair, finalPkHash));

        } else {

            // If we were invited based on non-expired terms, yet buyer
            // terms were not good enough, then we ditch conection.
            removeConnection(c->connectionId(), DisconnectCause::buyer_invited_with_bad_terms);
        }
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::tryToLoadPiece(detail::Connection<ConnectionIdType> * c) {

        assert(_session->state() == SessionState::started);
        assert(c-> template inState<joystream::protocol_statemachine::LoadingPiece>());


        if(c->loadedPiecePending()) {

            c->processEvent(joystream::protocol_statemachine::event::PieceLoaded(c->loadedPieceData()));
            c->setLoadedPiecePending(false);
        }

    }
}
}
}
