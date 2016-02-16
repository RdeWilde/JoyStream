/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#ifndef PAYMENT_CHANNEL_COMMITMENT_HPP
#define PAYMENT_CHANNEL_COMMITMENT_HPP

#include <common/PublicKey.hpp>

namespace Coin {
    class P2SHScriptPubKey;
    class MultisigScriptPubKey;
    class TxOut;
}

namespace joystream {
namespace paymentchannel {

    class Commitment {

    public:

        // Default constructor
        Commitment();

        // Constructor based on members
        Commitment(int64_t value, const Coin::PublicKey & firstPk, const Coin::PublicKey & secondPk);

        // Copy constructor
        Commitment(const Commitment& o);

        // Assignement operator
        Commitment & operator=(const Commitment & o);

        // p2sh 2of2 multisig scriptPubKey controlling contract output
        Coin::P2SHScriptPubKey contractOutputScriptPubKey() const;

        // Generates contract output
        Coin::TxOut contractOutput() const;

        // 2o2 multisig scriptpubkey
        Coin::MultisigScriptPubKey redeemScript() const;

        // Getters and setters
        int64_t value() const;
        void setValue(int64_t value);

        Coin::PublicKey firstPk() const;
        void setFirstPk(const Coin::PublicKey & firstPk);

        Coin::PublicKey secondPk() const;
        void setSecondPk(const Coin::PublicKey & secondPk);

    private:

        // Funds allocated to output
        int64_t _value;

        // First public key controlling multisig output
        Coin::PublicKey _firstPk;

        // Second public key controlling multisig output
        Coin::PublicKey _secondPk;
    };

}
}

#endif // PAYMENT_CHANNEL_COMMITMENT_HPP
