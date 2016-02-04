/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol/wire/JoinContract.hpp>
#include <protocol/wire/MessageType.hpp>

namespace joystream {
namespace protocol {
namespace wire {

    MessageType JoinContract::messageType() const {
        return MessageType::join_contract;
    }

}
}
}
