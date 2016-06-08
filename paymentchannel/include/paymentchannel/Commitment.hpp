/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#ifndef PAYMENT_CHANNEL_COMMITMENT_HPP
#define PAYMENT_CHANNEL_COMMITMENT_HPP

#include <common/PublicKey.hpp>
#include <paymentchannel/RedeemScript.hpp>

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
        Commitment(int64_t value, const Coin::PublicKey & firstPk, const Coin::PublicKey & secondPk, uint32_t lockTime);

        // Copy constructor
        Commitment(const Commitment& o);

        // Assignement operator
        Commitment & operator=(const Commitment & o);

        // p2sh 2of2 multisig scriptPubKey controlling contract output
        Coin::P2SHScriptPubKey contractOutputScriptPubKey() const;

        // Generates contract output
        Coin::TxOut contractOutput() const;

        // Multiway paychan script
        RedeemScript redeemScript() const;

        // Getters and setters
        int64_t value() const;
        void setValue(int64_t value);

        Coin::PublicKey firstPk() const;
        void setFirstPk(const Coin::PublicKey & firstPk);

        Coin::PublicKey secondPk() const;
        void setSecondPk(const Coin::PublicKey & secondPk);

        void setLockTime(uint32_t lockTime);
        uint64_t lockTime() const;

    private:

        // Funds allocated to output
        int64_t _value;

        // First public key controlling multisig output
        Coin::PublicKey _firstPk;

        // Second public key controlling multisig output
        Coin::PublicKey _secondPk;

        // How long the output is locked before payor can spend it
        uint32_t _lockTime;
    };

}
}

#endif // PAYMENT_CHANNEL_COMMITMENT_HPP
