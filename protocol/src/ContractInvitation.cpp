/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#include <protocol/ContractInvitation.hpp>

namespace joystream {
namespace protocol {

    ContractInvitation::ContractInvitation()
        : _value(0) {
    }

    ContractInvitation::ContractInvitation(quint64 value, const Coin::PublicKey & contractPk, const Coin::PubKeyHash & finalPkHash)
        : _value(value)
        , _contractPk(contractPk)
        , _finalPkHash(finalPkHash) {
    }

    bool ContractInvitation::operator==(const ContractInvitation & rhs) const {

        return _value == rhs.value() &&
               _contractPk == rhs.contractPk() &&
               _finalPkHash == rhs.finalPkHash();
    }

    quint64 ContractInvitation::value() const {
        return _value;
    }

    void ContractInvitation::setValue(quint64 value) {
        _value = value;
    }

    Coin::PublicKey ContractInvitation::contractPk() const {
        return _contractPk;
    }

    void ContractInvitation::setContractPk(const Coin::PublicKey & contractPk) {
        _contractPk = contractPk;
    }

    Coin::PubKeyHash ContractInvitation::finalPkHash() const {
        return _finalPkHash;
    }

    void ContractInvitation::setFinalPkHash(const Coin::PubKeyHash & finalPkHash) {
        _finalPkHash = finalPkHash;
    }
}
}
