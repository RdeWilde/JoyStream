/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#include <protocol_session/Session.hpp>
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
                                       const SellingPolicy & policy,
                                       const protocol_wire::SellerTerms & terms)
        : _session(session)
        , _removedConnection(removedConnection)
        , _generateKeyPairs(generateKeyPairs)
        , _generateP2PKHAddresses(generateP2PKHAddresses)
        , _policy(policy)
        , _terms(terms) {
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::removeConnection(const ConnectionIdType & id) {

        assert(_session->hasConnection(id))
        assert(_session->state() != SessionState::stopped); // We cannot have connection and be stopped

        // Do we need to do anything here? we may be owed payment
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::pieceLoaded(const protocol_wire::PieceData &, int index) {

    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::peerAnnouncedModeAndTerms(const ConnectionIdType & id, const protocol_statemachine::AnnouncedModeAndTerms & a) {
        // Nothing to do
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::invitedToOutdatedContract(const ConnectionIdType & id) {

    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::invitedToJoinContract(const ConnectionIdType & id) {

    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::contractPrepared(const ConnectionIdType & id, const Coin::typesafeOutPoint & anchor) {

    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::pieceRequested(const ConnectionIdType & id, int index) {

    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::invalidPieceRequested(const ConnectionIdType & id) {

    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::paymentInterrupted(const ConnectionIdType & id) {

    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::receivedValidPayment(const ConnectionIdType & id, const Coin::Signature & payment) {

    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::receivedInvalidPayment(const ConnectionIdType & id, const Coin::Signature & invalidPayment) {

    }

    template<class ConnectionIdType>
    Observing<ConnectionIdType> * Selling<ConnectionIdType>::toObserveMode() {

    }

    template<class ConnectionIdType>
    Buying<ConnectionIdType> * Selling<ConnectionIdType>::toBuyMode() {

    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::start() {

    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::stop() {

    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::pause() {

        // what if we pause when piece is being loaded, we
        // still send?

    }

    template <class ConnectionIdType>
    void Selling<ConnectionIdType>::tick() {

        // perhaps we are owed something, and
        // we may want to triggers ome timer
        // if w havent gotten paid in some amount of
        // time set by policy
    }

    template<class ConnectionIdType>
    void Selling<ConnectionIdType>::updateTerms(const protocol_wire::SellerTerms & terms) {
        _terms = terms;
    }

    template<class ConnectionIdType>
    protocol_wire::SellerTerms Selling<ConnectionIdType>::terms() const {
        return _terms;
    }
}
}
}
