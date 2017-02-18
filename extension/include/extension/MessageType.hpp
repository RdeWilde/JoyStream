/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_MESSAGE_TYPE_HPP
#define JOYSTREAM_EXTENSION_MESSAGE_TYPE_HPP

#include <string>

namespace joystream {
namespace extension {

    enum class MessageType {
        observe,
        buy,
        sell,
        join_contract,
        joining_contract,
        ready,
        request_full_piece,
        full_piece,
        payment
    };

    // Get name of message type
    const char * messageName(MessageType type);

    // Get type based on name
    MessageType messageType(const std::string &);
}
}

#endif // JOYSTREAM_EXTENSION_MESSAGE_TYPE_HPP
