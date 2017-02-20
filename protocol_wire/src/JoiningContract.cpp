/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/JoiningContract.hpp>

#include <common/PublicKey.hpp>

namespace joystream {
namespace protocol_wire {

    JoiningContract::JoiningContract() {
    }

    JoiningContract::JoiningContract(const Coin::PublicKey & contractPk, const Coin::PubKeyHash &finalPkHash)
        : _contractPk(contractPk)
        , _finalPkHash(finalPkHash) {
    }

    bool JoiningContract::operator==(const JoiningContract & rhs) const {
        return rhs.contractPk() == rhs.contractPk() &&
               rhs.finalPkHash() == rhs.finalPkHash();
    }

    Coin::PublicKey JoiningContract::contractPk() const {
        return _contractPk;
    }

    Coin::PubKeyHash JoiningContract::finalPkHash() const {
        return _finalPkHash;
    }

    void JoiningContract::setContractPk(const Coin::PublicKey &pk) {
        _contractPk = pk;
    }

    void JoiningContract::setFinalPkHash(const Coin::PubKeyHash & pkHash) {
        _finalPkHash = pkHash;
    }

}
}
