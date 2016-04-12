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

        Coin::PublicKey contractPk;
        Coin::operator >> (stream, contractPk);

        Coin::PubKeyHash finalPKHash;
        Coin::operator >> (stream, finalPKHash);

        // Store in invitation
        _invitation.setValue(value);
        _invitation.setContractPk(contractPk);
        _invitation.setFinalPkHash(finalPKHash);

        //// Index
        stream >> _index;
    }

    quint32 JoinContract::length() const {
        return (sizeof(quint64) + Coin::PublicKey::length() + Coin::PublicKey::length()) + sizeof(_index);
    }

    void JoinContract::write(QDataStream & stream) const {

        stream << _invitation.value();
        Coin::operator << (stream, _invitation.contractPk());
        Coin::operator << (stream, _invitation.finalPkHash());
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
