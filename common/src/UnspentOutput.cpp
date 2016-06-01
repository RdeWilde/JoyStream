/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/UnspentOutput.hpp>

namespace Coin {

UnspentOutput::UnspentOutput()
    : _value(0){
}

UnspentOutput::UnspentOutput(const KeyPair & keyPair, const typesafeOutPoint & output, quint64 value, uchar_vector scriptPubKey, uchar_vector redeemScript)
    : _keyPair(keyPair)
    , _outPoint(output)
    , _value(value)
    , _scriptPubKey(scriptPubKey)
    , _redeemScript(redeemScript) {
}

bool UnspentOutput::operator==(const UnspentOutput & o) const {

    return _value == o.value() &&
           _outPoint == o.outPoint() &&
           _keyPair == o.keyPair() &&
           _scriptPubKey == o.scriptPubKey() &&
           _redeemScript == o.redeemScript();
}

bool UnspentOutput::operator!=(const UnspentOutput & o) const {
    return !(*this == o);
}

KeyPair UnspentOutput::keyPair() const {
    return _keyPair;
}

void UnspentOutput::setKeyPair(const KeyPair &keyPair) {
    _keyPair = keyPair;
}

typesafeOutPoint UnspentOutput::outPoint() const {
    return _outPoint;
}

void UnspentOutput::setOutPoint(const typesafeOutPoint &outPoint) {
    _outPoint = outPoint;
}

quint64 UnspentOutput::value() const {
    return _value;
}

void UnspentOutput::setValue(quint64 value) {
    _value = value;
}

uchar_vector UnspentOutput::scriptPubKey() const {
    return _scriptPubKey;
}

void UnspentOutput::setScriptPubKey(uchar_vector script) {
    _scriptPubKey = script;
}

uchar_vector UnspentOutput::redeemScript() const {
    return _redeemScript;
}

void UnspentOutput::setRedeemScript(uchar_vector script) {
    _redeemScript = script;
}

}
