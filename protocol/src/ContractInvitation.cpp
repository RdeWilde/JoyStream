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

    ContractInvitation::ContractInvitation(int64_t value, const Coin::PublicKey & contractPk, const Coin::PublicKey & finalPk)
        : _value(value)
        , _contractPk(contractPk)
        , _finalPk(finalPk) {
    }

    int64_t ContractInvitation::value() const {
        return _value;
    }

    void ContractInvitation::setValue(int64_t value) {
        _value = value;
    }

    Coin::PublicKey ContractInvitation::contractPk() const {
        return _contractPk;
    }

    void ContractInvitation::setContractPk(const Coin::PublicKey & contractPk) {
        _contractPk = contractPk;
    }

    Coin::PublicKey ContractInvitation::finalPk() const {
        return _finalPk;
    }

    void ContractInvitation::setFinalPk(const Coin::PublicKey & finalPk) {
        _finalPk = finalPk;
    }
}
}
