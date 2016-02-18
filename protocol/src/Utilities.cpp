/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 18 2016
 */

#include <protocol/Utilities.hpp>
#include <paymentchannel/Payor.hpp>
#include <paymentchannel/Payee.hpp>

namespace joystream {
namespace protocol {
namespace utilities {

    joystream::paymentchannel::Payor createPayorForNewBuyer(const Coin::UnspentP2PKHOutput & utxo, Coin::KeyPair changeOutputKeyPair, quint64 changeValue, quint32 numberOfSellers) {

        // min_funds per channel = max( dust limit + fee required to refund output, being able to pay fo full thing at my own price point with one channel)
        // use total utxo balance and changevalue to figure out if we have neough to cover min_funds*#channels
        // coin::keypair payorcontractkeypair
        // coin::keypair payor finalkeypair

        const Coin::UnspentP2PKHOutput & utxo;

        Coin::KeyPair changeOutputKeyPair;

        quint64 changeValue;


        joystream::paymentchannel::Channel channel();

        return joystream::paymentchannel::Payor(channels,
                                                utxo,
                                                changeOutputKeyPair,
                                                changeValue,
                                                0,
                                                Coin::Transaction());
    }

    joystream::paymentchannel::Payee createPayeeForNewSeller(quint64 price, quint32 lock, quint64 settlementFee, const Coin::KeyPair & payeeContractKeys, const Coin::KeyPair & payeePaymentKeys) {

        return joystream::paymentchannel::Payee(0,
                                                lock,
                                                price,
                                                0,
                                                settlementFee,
                                                0,
                                                Coin::typesafeOutPoint(),
                                                payeeContractKeys,
                                                payeePaymentKeys,
                                                Coin::PublicKey(),
                                                Coin::PublicKey(),
                                                Coin::Signature());
    }




}
}
}
