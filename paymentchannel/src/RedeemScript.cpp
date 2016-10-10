/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>
 */

#include <paymentchannel/RedeemScript.hpp>
#include <common/PublicKey.hpp>
#include <common/Utilities.hpp>

namespace joystream {
namespace paymentchannel {

RedeemScript::RedeemScript(const Coin::PublicKey & payorPk, const Coin::PublicKey & payeePk, uint16_t relativeLockTime)
    : _payorPk(payorPk)
    , _payeePk(payeePk)
    , _lockTime(relativeLockTime) {

}

uchar_vector RedeemScript::serialized() const {
    uchar_vector script;

    script.push_back(0x63); // OP_IF
    // Branch for when channel is settled with payment and refund
    script += Coin::opPushData(Coin::PublicKey::length());
    script += _payeePk.toUCharVector();
    script.push_back(0xad); // OP_CHECKSIGVERIFY

    script.push_back(0x67); // OP_ELSE
    // Branch for when channel is settled with full refund to payor
    uchar_vector locktime = dataCSVRelativeLockTime(_lockTime);
    script += Coin::opPushData(locktime.size());
    script += locktime;
    script.push_back(0xb2); // OP_CHECKSEQUENCEVERIFY (BIP 68)
    script.push_back(0x75); // OP_DROP

    script.push_back(0x68); // OP_ENDIF
    // Check that payor has agreed to this spend
    script += Coin::opPushData(Coin::PublicKey::length());
    script += _payorPk.toUCharVector();
    script.push_back(0xac); // OP_CHECKSIG

    return script;
}

RedeemScript RedeemScript::deserialize(const uchar_vector & script) {
    // check size (minimum size when locktime is encoded as a single byte)
    if(script.size() < 76) {
        throw std::runtime_error("redeem script too short");
    }

    // get a subscript starting at expected payee public key push data operation
    uchar_vector subscript(script.begin() + 1, script.end());

    uchar_vector rawPayeePk;

    subscript = Coin::popData(subscript, rawPayeePk);

    if(rawPayeePk.size() != Coin::PublicKey::length()) {
        throw std::runtime_error("Unable to retreive payee public key from redeem script");
    }

    Coin::PublicKey payeePk(rawPayeePk);

    // get a subscript to the start of the locktime push data operation
    subscript = uchar_vector(subscript.begin() + 2, subscript.end());

    uchar_vector rawLockTime;

    subscript = Coin::popData(subscript, rawLockTime);

    if(rawLockTime.empty()) {
        throw std::runtime_error("Unable to retreive Locktime from redeem script");
    }

    // decode the locktime
    uint16_t locktime = Coin::deserializeScriptNum(rawLockTime);

    // get a subscript to the start of the payor publick key push data operation
    subscript = uchar_vector(subscript.begin() + 3, subscript.end());

    uchar_vector rawPayorPk;
    subscript = Coin::popData(subscript, rawPayorPk);

    if(rawPayorPk.size() != Coin::PublicKey::length()) {
        throw std::runtime_error("Unable to retreive payor public key from redeem script");
    }

    Coin::PublicKey payorPk(rawPayorPk);

    RedeemScript result(payorPk, payeePk, locktime);

    if(result.serialized() == script) {
        return result;
    }

    throw std::runtime_error("script is not a valid paychan script");

}

uchar_vector RedeemScript::PayorOptionalData() {
    return uchar_vector(0x00); /* OP_FALSE */
}

uchar_vector RedeemScript::PayeeOptionalData() {
    return uchar_vector(0x01); /* OP_TRUE */
}

uchar_vector RedeemScript::dataCSVRelativeLockTime(const uint16_t blocks) {
    return Coin::serializeScriptNum(blocks);
}

}}
