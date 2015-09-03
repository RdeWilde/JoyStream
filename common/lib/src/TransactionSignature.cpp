/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#include <common/TransactionSignature.hpp>

namespace Coin {

TransactionSignature::TransactionSignature(const Signature & sig, SigHashType type)
    : _sig(sig)
    , _type(type) {
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
