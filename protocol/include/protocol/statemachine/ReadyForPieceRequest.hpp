/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_READYFORPIECEREQUEST_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_READYFORPIECEREQUEST_HPP

#include <protocol/statemachine/Selling.hpp>
#include <protocol/statemachine/event/Recv.hpp>
#include <protocol/wire/RequestFullPiece.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class ReadyForPieceRequest : public sc::simple_state<ReadyForPieceRequest, Selling> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<wire::RequestFullPiece>>
                                > reactions;

        ReadyForPieceRequest();

        // Event handlers
        sc::result react(const event::Recv<wire::RequestFullPiece> &);

    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_READYFORPIECEREQUEST_HPP
