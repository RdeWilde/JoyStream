/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 18 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SESSION_BUYER_SESSION_HPP
#define JOYSTREAM_PROTOCOL_SESSION_BUYER_SESSION_HPP

#include <QtGlobal> // q primitive types

#include <common/PubKeyHash.hpp> // Hard to forward declare typedefined PubKeyHash

namespace Coin {
    class UnspentP2PKHOutput;
    class P2PKHAddress;
    class KeyPair;
}

namespace joystream {

    namespace paymentchannel {
        class Payor;
        class Payee;
    }

    namespace protocol {
    namespace utilities {

        /**
         * Paymentchannel routines
         */

        // Payor as seen by a fresh buyer session
        joystream::paymentchannel::Payor createPayorForNewBuyer(const Coin::UnspentP2PKHOutput & utxo, const Coin::P2PKHAddress & changeAddress);

        // Payee as seen by a fresh seller connection
        joystream::paymentchannel::Payee createPayeeForNewSeller(quint64 price, quint32 lock, quint64 settlementFee, const Coin::KeyPair & payeeContractKeys, const Coin::PubKeyHash & payeeFinalPubKeyHash);

    }
    }
}

#endif // JOYSTREAM_PROTOCOL_SESSION_BUYER_SESSION_HPP
