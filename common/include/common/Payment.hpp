/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 5 2015
 */

#ifndef COIN_PAYMENT_HPP
#define COIN_PAYMENT_HPP

#include <common/RedeemScriptHash.hpp>

namespace Coin {

    class TxOut;

    class Payment {

    public:


        Payment();

        Payment(int64_t value, const Coin::RedeemScriptHash & destination);

        // Corresponding P2SH transaction output
        Coin::TxOut txOut() const;

        // Getters and setters
        int64_t value() const;
        void setValue(int64_t value);

        Coin::RedeemScriptHash destination() const;
        void setDestination(const Coin::RedeemScriptHash & destination);

    private:

        // Size of payment (#satoshi)
        int64_t _value;

        // Recipient
        Coin::RedeemScriptHash _destination;

    };

}

#endif // COMMON_PAYMENT_HPP

