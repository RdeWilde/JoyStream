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

    JoinContract::JoinContract()
        : _index(0) {
    }

    JoinContract::JoinContract(const ContractInvitation & invitation, uint32_t index)
        : _invitation(invitation)
        , _index(index) {
    }

    JoinContract::JoinContract(QDataStream & stream) {

        //// Invitation

        // Read from stream into temporary variables
        quint64 value;
        stream >> value;

        Coin::PublicKey contractPk, finalPk;
        Coin::operator >> (stream, contractPk);
        Coin::operator >> (stream, finalPk);

        // Store in invitation
        _invitation.setValue(value);
        _invitation.setContractPk(contractPk);
        _invitation.setFinalPk(finalPk);

        //// Index
        stream >> _index;
    }

    quint32 JoinContract::length() const {
        return (sizeof(quint64) + Coin::PublicKey::length() + Coin::PublicKey::length()) + sizeof(_index);
    }

    void JoinContract::write(QDataStream & stream) const {

        stream << _invitation.value();
        Coin::operator << (stream, _invitation.contractPk());
        Coin::operator << (stream, _invitation.finalPk());
        stream << _index;
    }

    MessageType JoinContract::messageType() const {
        return MessageType::join_contract;
    }

    ContractInvitation JoinContract::invitation() const {
        return _invitation;
    }

    void JoinContract::setInvitation(const ContractInvitation & invitation) {
        _invitation = invitation;
    }

    uint32_t JoinContract::index() const {
        return _index;
    }

    void JoinContract::setIndex(uint32_t index) {
        _index = index;
    }
}
}
}
