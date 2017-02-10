/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_EXTENDEDMESSAGEPAYLOAD_HPP
#define JOYSTREAM_PROTOCOL_WIRE_EXTENDEDMESSAGEPAYLOAD_HPP

#include <cstdint>
#include <iostream>

namespace joystream {
namespace protocol_wire {

    enum class MessageType;

    class ExtendedMessagePayload {

    public:

        // Required for safe deletion through pointer of this base type
        virtual ~ExtendedMessagePayload() {};

        // Get type of message
        virtual MessageType messageType() const = 0;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_EXTENDEDMESSAGEPAYLOAD_HPP
