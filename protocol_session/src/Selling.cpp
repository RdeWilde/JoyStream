/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#include <protocol_session/Selling.hpp>

namespace joystream {
namespace protocol_session {

    template <class ConnectionIdType>
    Selling<ConnectionIdType>::Selling(const RemovedConnectionCallbackHandler<ConnectionIdType> & removedConnectionCallbackHandler,
                                       const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                       const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler)
        : _sessionCore(removedConnectionCallbackHandler,
                       generateKeyPairsCallbackHandler,
                       generateP2PKHAddressesCallbackHandler){

        // Note starting time
        //time(&_lastStart);
    }

    template <class ConnectionIdType>
    void Selling<ConnectionIdType>::tick() {

    }

    template<class ConnectionIdType>
    uint Selling<ConnectionIdType>::addConnection(const ConnectionIdType & id, const SendMessageOnConnection &) {

    }

    template<class ConnectionIdType>
    uint Selling<ConnectionIdType>::removeConnection(const ConnectionIdType & id) {

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
    void Selling<ConnectionIdType>::pieceRequested(const ConnectionIdType & id, int i) {

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

}
}
