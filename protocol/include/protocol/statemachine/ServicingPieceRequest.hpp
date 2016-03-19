/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_SERVICINGPIECEREQUEST_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_SERVICINGPIECEREQUEST_HPP

#include <protocol/statemachine/Selling.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class ServicingPieceRequest : public sc::simple_state<ServicingPieceRequest, Selling> {

    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_SERVICINGPIECEREQUEST_HPP

