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

    ContractRSVP::ContractRSVP(const Coin::PublicKey & contractPk, const Coin::PublicKey & finalPk)
        : _contractPk(contractPk)
        , _finalPk(finalPk) {
    }

    bool ContractRSVP::operator==(const ContractRSVP & rhs) const {
        return _contractPk == rhs.contractPk() &&
               _finalPk == rhs.finalPk();
    }

    Coin::PublicKey ContractRSVP::contractPk() const {
        return _contractPk;
    }

    void ContractRSVP::setContractPk(const Coin::PublicKey & contractPk) {
        _contractPk = contractPk;
    }

    Coin::PublicKey ContractRSVP::finalPk() const {
        return _finalPk;
    }

    void ContractRSVP::setFinalPk(const Coin::PublicKey &finalPk) {
        _finalPk = finalPk;
    }
}
}
