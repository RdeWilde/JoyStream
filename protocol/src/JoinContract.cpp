/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol/JoinContract.hpp>
#include <protocol/MessageType.hpp>

namespace joystream {
namespace protocol {

    MessageType JoinContract::messageType() const {
        return MessageType::join_contract;
    }

}
}
