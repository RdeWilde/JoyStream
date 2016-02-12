/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#include <protocol/Session.hpp>

namespace joystream {
namespace protocol {

    Session::Session(Mode mode,
                     const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                     const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                     const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler)
        : _mode(mode)
        , _removedConnectionCallbackHandler(removedConnectionCallbackHandler)
        , _generateKeyPairsCallbackHandler(generateKeyPairsCallbackHandler)
        , _generateP2PKHAddressesCallbackHandler(generateP2PKHAddressesCallbackHandler) {
    }

    Mode Session::mode() const {
        return _mode;
    }

}
}
