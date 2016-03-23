/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 18 2016
 */

#include <protocol/ContractRSVP.hpp>

namespace joystream {
namespace protocol {

    ContractRSVP::ContractRSVP() {
    }

    ContractRSVP::ContractRSVP(const Coin::PublicKey & contractPk, const Coin::PubKeyHash & finalPkHash)
        : _contractPk(contractPk)
        , _finalPkHash(finalPkHash) {
    }

    bool ContractRSVP::operator==(const ContractRSVP & rhs) const {
        return _contractPk == rhs.contractPk() &&
               _finalPkHash == rhs.finalPkHash();
    }

    Coin::PublicKey ContractRSVP::contractPk() const {
        return _contractPk;
    }

    void ContractRSVP::setContractPk(const Coin::PublicKey & contractPk) {
        _contractPk = contractPk;
    }

    Coin::PubKeyHash ContractRSVP::finalPkHash() const {
        return _finalPkHash;
    }

    void ContractRSVP::setFinalPkHash(const Coin::PubKeyHash & finalPkHash) {
        _finalPkHash = finalPkHash;
    }
}
}
