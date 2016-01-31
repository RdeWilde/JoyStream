/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#include <paymentchannel/Commitment.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/MultisigScriptPubKey.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::TxOut


namespace joystream {
namespace paymentchannel {

    Commitment::Commitment()
        : _value(0) {
    }

    Commitment::Commitment(int64_t value, const Coin::PublicKey & firstPk, const Coin::PublicKey & secondPk)
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

    Coin::P2SHScriptPubKey Commitment::contractOutputScriptPubKey() const {
        return Coin::P2SHScriptPubKey::fromMultisig(std::vector<Coin::PublicKey>({_firstPk, _secondPk}), 2);
    }

    Coin::TxOut Commitment::contractOutput() const {
        return Coin::TxOut(_value, contractOutputScriptPubKey().serialize());
    }

    Coin::MultisigScriptPubKey Commitment::redeemScript() const {
        return Coin::MultisigScriptPubKey(std::vector<Coin::PublicKey>({_firstPk, _secondPk}), 2);
    }

    int64_t Commitment::value() const {
        return _value;
    }

    void Commitment::setValue(int64_t value) {
        _value = value;
    }

    Coin::PublicKey Commitment::firstPk() const {
        return _firstPk;
    }

    void Commitment::setFirstPk(const Coin::PublicKey & firstPk) {
        _firstPk = firstPk;
    }

    Coin::PublicKey Commitment::secondPk() const {
        return _secondPk;
    }

    void Commitment::setSecondPk(const Coin::PublicKey & secondPk) {
        _secondPk = secondPk;
    }

}
}
