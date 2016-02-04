/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PROTOCOL_OBSERVE_HPP
#define PROTOCOL_OBSERVE_HPP

#include <protocol/wire/NoPayloadMessage.hpp>

namespace joystream {
namespace protocol {
namespace wire {

    class Observe : public NoPayloadMessage {

    public:
        virtual MessageType messageType() const;
    };

}
}
}

#endif // PROTOCOL_OBSERVE_HPP
