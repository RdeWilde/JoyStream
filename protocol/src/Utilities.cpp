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

    joystream::paymentchannel::Payor createPayorForNewBuyer(const Coin::UnspentP2PKHOutput & utxo, const Coin::P2PKHAddress & changeAddress) {

        return joystream::paymentchannel::Payor(std::vector<joystream::paymentchannel::Channel>(),
                                                utxo,
                                                changeAddress,
                                                0,
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
