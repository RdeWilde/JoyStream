/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol/wire/JoiningContract.hpp>
#include <protocol/wire/MessageType.hpp>

#include <QDataStream>
#include <common/PublicKey.hpp>

namespace joystream {
namespace protocol {
namespace wire {

    JoiningContract::JoiningContract() {
    }

    JoiningContract::JoiningContract(const ContractRSVP & rsvp)
        : _rsvp(rsvp) {
    }

    JoiningContract::JoiningContract(QDataStream & stream) {

        Coin::PublicKey contractPk, finalPk;

        Coin::operator >> (stream, contractPk);
        Coin::operator >> (stream, finalPk);

        _rsvp.setContractPk(contractPk);
        _rsvp.setFinalPk(finalPk);
    }

    ContractRSVP JoiningContract::rsvp() const {
        return _rsvp;
    }

    void JoiningContract::setRsvp(const ContractRSVP & rsvp) {
        _rsvp = rsvp;
    }

    MessageType JoiningContract::messageType() const {
        return MessageType::joining_contract;
    }

    quint32 JoiningContract::length() const {
        return Coin::PublicKey::length() + Coin::PublicKey::length();
    }

    void JoiningContract::write(QDataStream & stream) const {

        // DOESN'T LINK: stream << _contractPk << _finalPk;
        Coin::operator << (stream, _rsvp.contractPk());
        Coin::operator << (stream, _rsvp.finalPk());
    }

}
}
}
