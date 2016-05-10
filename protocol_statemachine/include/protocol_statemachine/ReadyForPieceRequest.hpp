/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_READYFORPIECEREQUEST_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_READYFORPIECEREQUEST_HPP

#include <protocol_statemachine/Selling.hpp>
#include <protocol_statemachine/event/Recv.hpp>
#include <protocol_wire/protocol_wire.hpp>

namespace joystream {
namespace protocol_statemachine {

    class ReadyForPieceRequest : public sc::simple_state<ReadyForPieceRequest, Selling> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<protocol_wire::RequestFullPiece>>
                                > reactions;

        ReadyForPieceRequest();

        // Event handlers
        sc::result react(const event::Recv<protocol_wire::RequestFullPiece> &);

    };

}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_READYFORPIECEREQUEST_HPP
