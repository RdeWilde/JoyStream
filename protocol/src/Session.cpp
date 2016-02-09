/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#include <protocol/Session.hpp>

namespace joystream {
namespace protocol {

    Session::Session(Mode mode, const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler)
        : _mode(mode)
        , _removedConnectionCallbackHandler(removedConnectionCallbackHandler) {
    }

    Mode Session::mode() const {
        return _mode;
    }

}
}
