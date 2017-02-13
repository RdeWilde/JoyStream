/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_OBSERVE_HPP
#define JOYSTREAM_WIRE_OBSERVE_HPP

#include <protocol_wire/Message.hpp>

namespace joystream {
namespace protocol_wire {

    class Observe : public Message {

    public:
        virtual ~Observe() {}
        virtual MessageType messageType() const;
    };

}
}

#endif // JOYSTREAM_WIRE_OBSERVE_HPP
