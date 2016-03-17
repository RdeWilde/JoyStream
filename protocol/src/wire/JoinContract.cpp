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

    JoinContract::JoinContract() {
    }

    JoinContract::JoinContract(const ContractInvitation & invitation)
        : _invitation(invitation) {
    }

    JoinContract::JoinContract(QDataStream & stream) {

        // Read in variables
        quint64 value;
        Coin::PublicKey contractPk, finalPk;

        // Read
        stream >> value;
        Coin::operator >> (stream, contractPk);
        Coin::operator >> (stream, finalPk);

        // Store in invitation
        _invitation.setValue(value);
        _invitation.setContractPk(contractPk);
        _invitation.setFinalPk(finalPk);
    }

    quint32 JoinContract::length() const {
        return sizeof(quint64) + Coin::PublicKey::length() + Coin::PublicKey::length();
    }

    void JoinContract::write(QDataStream & stream) const {

        stream << _invitation.value();
        Coin::operator << (stream, _invitation.contractPk());
        Coin::operator << (stream, _invitation.finalPk());
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
}
}
}
