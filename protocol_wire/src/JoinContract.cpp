/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/JoinContract.hpp>
#include <protocol_wire/MessageType.hpp>

#include <QDataStream>

namespace joystream {
namespace protocol_wire {

    JoinContract::JoinContract()
        : _index(0) {
    }


    JoinContract::JoinContract(uint32_t index)
        : _index(index) {
    }

    JoinContract::JoinContract(QDataStream & stream) {
        stream >> _index;
    }

    quint32 JoinContract::length() const {
        return sizeof(_index);
    }

    void JoinContract::write(QDataStream & stream) const {
        stream << _index;
    }

    MessageType JoinContract::messageType() const {
        return MessageType::join_contract;
    }

    uint32_t JoinContract::index() const {
        return _index;
    }

}
}
