/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#include <core/extension/PaymentChannel/Commitment.hpp>
#include <common/P2SHAddress.hpp>
//#include <CoinCore/StandardTransactions.h>
#include <CoinCore/hash.h> // ripemd160, sha256

namespace Coin {

Commitment::Commitment()
    : _value(0) {
}

Commitment::Commitment(quint64 value, const PublicKey & firstPk, const PublicKey & secondPk)
    : _value(value)
    , _firstPk(firstPk)
    , _secondPk(secondPk) {
}

Commitment::Commitment(const Commitment & o)
    : Commitment::Commitment(o.value(), o.firstPk(), o.secondPk()) {
}

Commitment & Commitment::operator=(const Commitment & o) {

    _value = o.value();
    _firstPk = o.firstPk();
    _secondPk =  o.secondPk();

    return *this;
}

// Corresponding P2SH 2-2 multisig address
//Coin::P2SHAddress toP2SHAddress(Network network) const {
//
// generate MultiSigRedeemScript, create address
//
//}

/**
// Corresponding transaction output
Coin::StandardTxOut Commitment::toTxOut() const {


    // Create 2-2 multisig redeem script (scriptPubKey)
    // We dont care about address parameter, since we are
    // just going to extract redeem script anyway
    Coin::MultiSigRedeemScript TwoOfTwoScriptPubkKey(2);
    TwoOfTwoScriptPubkKey.addPubKey(_firstPk.toUCharVector());
    TwoOfTwoScriptPubkKey.addPubKey(_secondPk.toUCharVector());

    // Hash of serialized 2-2 redeem script
    uchar_vector scriptHash = ripemd160(sha256(TwoOfTwoScriptPubkKey.getRedeemScript()));

    Q_ASSERT(scriptHash.size() == P2SHAddress::redeemScriptLength);

    return Coin::StandardTxOut();

}
*/

quint64 Commitment::value() const {
    return _value;
}

void Commitment::setValue(quint64 value) {
    _value = value;
}

PublicKey Commitment::firstPk() const {
    return _firstPk;
}

void Commitment::setFirstPk(const PublicKey & firstPk) {
    _firstPk = firstPk;
}

PublicKey Commitment::secondPk() const {
    return _secondPk;
}

void Commitment::setSecondPk(const PublicKey & secondPk) {
    _secondPk = secondPk;
}

}
