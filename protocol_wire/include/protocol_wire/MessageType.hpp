/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_MESSAGE_TYPE_HPP
#define JOYSTREAM_WIRE_MESSAGE_TYPE_HPP

#include <string>

namespace joystream {
namespace protocol_wire {

    enum class MessageType {
        observe,
        buy,
        sell,
        join_contract,
        joining_contract,
        sign_refund,
        refund_signed,
        ready,
        request_full_piece,
        full_piece,
        payment //, end
    };


    // Get name of message type
    const char * messageName(MessageType type);

    // Get type based on name
    MessageType messageType(const std::string &);
}
}

#endif // JOYSTREAM_WIRE_MESSAGE_TYPE_HPP
