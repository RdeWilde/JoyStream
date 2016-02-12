/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#include <protocol/sell/Session.hpp>

namespace joystream {
namespace protocol {
namespace sell {

    Session::Session(const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                    const std::map<std::string, Connection> & connections,
                    const Terms & terms,
                    uint32_t numberOfPiecesInTorrent)
        : joystream::protocol::Session(Mode::sell, removedConnectionCallbackHandler)
        , _connections(connections)
        , _terms(terms)
        , _numberOfPiecesInTorrent(numberOfPiecesInTorrent) {
    }

    void Session::updateTerms(const sell::Terms & terms) {

    }

    void Session::switchToBuyMode(const buy::Terms & terms) {

    }

    void Session::switchToObserveMode() {

    }
}
}
}
