/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#include <common/TransactionSignature.hpp>
#include <common/Utilities.hpp> // opPushData

namespace Coin {

TransactionSignature::TransactionSignature(const Signature & sig, const SigHashType & type)
    : _sig(sig)
    , _type(type) {
}

uchar_vector TransactionSignature::serializeForScriptSig() const {

    uchar_vector serialize;

    // Add signature length indicator
    serialize += opPushData(_sig.length());

    // Add signature
    serialize += _sig.toUCharVector();

    // Add sighash type flag
    serialize += opPushData(_type.hashCode());

    return serialize;
}

Signature TransactionSignature::sig() const {
    return _sig;
}

void TransactionSignature::setSig(const Signature & sig) {
    _sig = sig;
}

SigHashType TransactionSignature::type() const {
    return _type;
}

void TransactionSignature::setType(SigHashType type) {
    _type = type;
}

}
