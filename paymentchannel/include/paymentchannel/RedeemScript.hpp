/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>
 */

#ifndef PAYMENT_CHANNEL_REDEEMSCRIPT_H
#define PAYMENT_CHANNEL_REDEEMSCRIPT_H

#include <common/PublicKey.hpp>
#include <stdutils/uchar_vector.h>

namespace Coin {
    class PublicKey;
}

namespace joystream {
namespace paymentchannel {

class RedeemScript
{
public:
    explicit RedeemScript(const Coin::PublicKey & payorPk, const Coin::PublicKey & payeePk, uint16_t relativeLockTime);

    uchar_vector serialized() const;

    static RedeemScript deserialize(const uchar_vector & script);

    // Optional data that must used in the scriptSig to spend the p2sh output in a refund tx by the Payor (Buyer)
    static uchar_vector PayorOptionalData();

    // Optional data that must used in the scriptSig to spend the p2sh output in the settlement tx by the Payee (Seller)
    static uchar_vector PayeeOptionalData();

    // Data to be pushed to stack to be used with OP_CHECKSEQUENCEVERIFY
    static uchar_vector dataCSVRelativeLockTime_Blocks(const uint16_t blocks);

    // Data to be pushed to stack to be used with OP_CHECKSEQUENCEVERIFY
    static uchar_vector dataCSVRelativeLockTime_Time(const uint16_t time);


    // Returns correct nSequence value to use for the transaction input
    static uint32_t nSequence_Blocks(const uint16_t blocks);
    static uint32_t nSequence_Time(const uint16_t time);

private:
    Coin::PublicKey _payorPk;
    Coin::PublicKey _payeePk;

    // Relative locktime
    uint16_t _lockTime;
};

}}
#endif // PAYMENTCHANNEL_REDEEMSCRIPT_H
