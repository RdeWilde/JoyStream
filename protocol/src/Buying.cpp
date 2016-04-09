/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#include <protocol/Buying.hpp>
#include <wire/PieceData.hpp>

namespace joystream {
namespace protocol {

    template <class ConnectionIdType>
    Buying<ConnectionIdType>::Buying(detail::SessionCoreImpl<ConnectionIdType> * sessionCore)
        : _sessionCore(sessionCore){

        // Note starting time
        time(&_lastStart);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::sellerHasJoined(const ConnectionIdType &) {

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::sellerHasInterruptedContract(const ConnectionIdType &) {

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::receivedFullPiece(const ConnectionIdType &, const joystream::wire::PieceData &) {

    }

}
}
