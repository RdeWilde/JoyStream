/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#ifndef JOYSTREAM_PROTOCOL_CONTRACTINVITATION_HPP
#define JOYSTREAM_PROTOCOL_CONTRACTINVITATION_HPP

#include <common/PublicKey.hpp>

namespace joystream {
namespace protocol {

    class ContractInvitation {

    public:

        ContractInvitation();

        ContractInvitation(int64_t value, const Coin::PublicKey & contractPk, const Coin::PublicKey & finalPk);

        // Getters and setters
        int64_t value() const;
        void setValue(int64_t value);

        Coin::PublicKey contractPk() const;
        void setContractPk(const Coin::PublicKey &);

        Coin::PublicKey finalPk() const;
        void setFinalPk(const Coin::PublicKey &);

    private:

        // Contract output value
        int64_t _value;

        // Contract output buyer multisig key
        Coin::PublicKey _contractPk;

        // Payment/Refund buyer output
        Coin::PublicKey _finalPk;

    };
}
}
#endif // JOYSTREAM_PROTOCOL_CONTRACTINVITATION_HPP

