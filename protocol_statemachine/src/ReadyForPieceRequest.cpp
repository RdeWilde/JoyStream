/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#include <protocol_statemachine/ReadyForPieceRequest.hpp>
#include <protocol_statemachine/ServicingPieceRequest.hpp>

namespace joystream {
namespace protocol_statemachine {

    ReadyForPieceRequest::ReadyForPieceRequest() {
        std::cout << "Entering ReadyForPieceRequest state." << std::endl;
    }

    sc::result ReadyForPieceRequest::react(const event::Recv<protocol_wire::RequestFullPiece> & e) {

        std::cout << "Reacting to Recv<wire::RequestFullPiece> event." << std::endl;

        // Check that piece requested is valid,
        int pieceIndex = e.message().pieceIndex();

        if(pieceIndex < 0 || pieceIndex > context<CBStateMachine>()._MAX_PIECE_INDEX) {

            // if not, send notification and terminate
            context<CBStateMachine>()._invalidPieceRequested();

            return terminate();
        }

        // otherwise send success notification, and
        context<CBStateMachine>()._pieceRequested(pieceIndex);

        // get ready to load the piece
        return transit<ServicingPieceRequest>();
    }

}
}
