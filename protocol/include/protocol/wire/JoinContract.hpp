/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PROTOCOL_JOIN_CONTRACT_HPP
#define PROTOCOL_JOIN_CONTRACT_HPP

#include <protocol/wire/NoPayloadMessage.hpp>

namespace joystream {
namespace protocol {
namespace wire {

    class JoinContract : public NoPayloadMessage {

    public:

        virtual MessageType messageType() const;
    };

}
}
}

#endif // PROTOCOL_JOIN_CONTRACT_HPP
