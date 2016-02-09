/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/buy/Session.hpp>

namespace joystream {
namespace protocol {
namespace buy {

    Session::Session(const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                    std::map<std::string, Connection> connections,
                    SessionState state,
                    const Terms & terms,
                    const joystream::paymentchannel::Payor & payor,
                    const std::vector<Piece> & pieces)
        : joystream::protocol::Session(Mode::buy, removedConnectionCallbackHandler)
        , _connections(connections)
        , _state(state)
        , _terms(terms)
        , _payor(payor)
        , _pieces(pieces) {

        // _numberOfUnassignedPieces

        // _namesOfConnectionsWithoutPieceAssignment

        // _assignmentLowerBound
    }


    void updateTerms(const Terms & terms) {

    }

    void switchToSellMode(const joystream::protocol::sell::Terms & terms) {

    }

    void switchToObserveMode() {

    }

    void addConnection(const Connection & connection) {

    }

    void removeConnection(const std::string & name) {

    }

    void processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload * message) {

    }


}
}
}
