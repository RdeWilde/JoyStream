/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_CONTRACTINVITATION_HPP
#define JOYSTREAM_PROTOCOL_WIRE_CONTRACTINVITATION_HPP

#include <common/PublicKey.hpp>

namespace joystream {
namespace protocol_wire {

    class ContractInvitation {

    public:

        ContractInvitation();

        ContractInvitation(quint64 value, const Coin::PublicKey & contractPk, const Coin::PubKeyHash & finalPkHash);

        bool operator==(const ContractInvitation &) const;

        // Getters and setters
        quint64 value() const;
        void setValue(quint64 value);

        Coin::PublicKey contractPk() const;
        void setContractPk(const Coin::PublicKey &);

        Coin::PubKeyHash finalPkHash() const;
        void setFinalPkHash(const Coin::PubKeyHash &);

    private:

        // Contract output value
        // cant be int64_t, due to compatibility wiht QDataStream
        quint64 _value;

        // Contract output buyer multisig key
        Coin::PublicKey _contractPk;

        // Payment/Refund buyer output
        Coin::PubKeyHash _finalPkHash;
    };
}
}
#endif // JOYSTREAM_PROTOCOL_WIRE_CONTRACTINVITATION_HPP
