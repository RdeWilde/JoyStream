/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_PIECEREQUESTED_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_PIECEREQUESTED_HPP

#include <protocol_statemachine/ServicingPieceRequest.hpp>
#include <protocol_statemachine/event/PieceLoaded.hpp>

namespace joystream {
namespace protocol_statemachine {

    class LoadingPiece : public sc::simple_state<LoadingPiece, ServicingPieceRequest> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::PieceLoaded>
                                > reactions;

        LoadingPiece();

        // Event handlers
        sc::result react(const event::PieceLoaded &);
    };

}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_PIECEREQUESTED_HPP
