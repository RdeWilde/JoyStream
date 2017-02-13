/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/JoinContract.hpp>
#include <protocol_wire/MessageType.hpp>

namespace joystream {
namespace protocol_wire {

    JoinContract::JoinContract()
        : JoinContract(0) {
    }

    JoinContract::JoinContract(uint32_t index)
        : _index(index) {
    }

    bool JoinContract::operator==(const JoinContract & rhs) const {
        return _index == rhs.index();
    }

    MessageType JoinContract::messageType() const {
        return MessageType::join_contract;
    }

    uint32_t JoinContract::index() const {
        return _index;
    }

    void JoinContract::setIndex(uint32_t index) {
        _index = index;
    }

}
}
