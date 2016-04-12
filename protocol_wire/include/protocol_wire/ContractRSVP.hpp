/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 18 2016
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_CONTRACTRSVP_HPP
#define JOYSTREAM_PROTOCOL_WIRE_CONTRACTRSVP_HPP

#include <common/PublicKey.hpp>

namespace joystream {
namespace protocol_wire {

    class ContractRSVP {

    public:

        ContractRSVP();

        ContractRSVP(const Coin::PublicKey & contractPk, const Coin::PubKeyHash & finalPkHash);

        bool operator==(const ContractRSVP &) const;

        // Getters and setters
        Coin::PublicKey contractPk() const;
        void setContractPk(const Coin::PublicKey &);

        Coin::PubKeyHash finalPkHash() const;
        void setFinalPkHash(const Coin::PubKeyHash &);

    private:

        // Contract output seller multisig key
        Coin::PublicKey _contractPk;

        // Payment seller output
        Coin::PubKeyHash _finalPkHash;
    };
}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_CONTRACTRSVP_HPP
