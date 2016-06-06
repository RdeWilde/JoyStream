
/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/UnspentP2PKHOutput.hpp>
#include <CoinCore/CoinNodeData.h>
#include <common/Utilities.hpp>

namespace Coin {

UnspentP2PKHOutput::UnspentP2PKHOutput()
    : UnspentOutput() {
}

UnspentP2PKHOutput::UnspentP2PKHOutput(const KeyPair & keyPair, const typesafeOutPoint & outPoint, uint64_t setValue)
    : UnspentOutput(outPoint, setValue)
    , _keyPair(keyPair) {
}

bool UnspentP2PKHOutput::operator==(const UnspentP2PKHOutput & o) const {

    return _keyPair == o.keyPair() &&
           outPoint() == o.outPoint() &&
           value() == o.value();
}

bool UnspentP2PKHOutput::operator!=(const UnspentP2PKHOutput & o) const {
    return !(*this == o);
}

KeyPair UnspentP2PKHOutput::keyPair() const {
    return _keyPair;
}

void UnspentP2PKHOutput::setKeyPair(const KeyPair &keyPair) {
    _keyPair = keyPair;
}

uchar_vector UnspentP2PKHOutput::scriptPubKey() const {
    return P2PKHScriptPubKey(keyPair().pk()).serialize();
}

uchar_vector UnspentP2PKHOutput::scriptSig(const Transaction & tx, const SigHashType &sigHashType) const {

    // Transaction hash to be signed (sighash)
    uchar_vector sigHash = sighash(tx, outPoint(), scriptPubKey(), sigHashType);

    // Sign the sighash with the private key
    TransactionSignature sig(keyPair().sk().sign(sigHash), sigHashType);

    uchar_vector scriptSig;

    scriptSig += sig.opPushForScriptSigSerialized();

    scriptSig += opPushData(0x21);
    scriptSig += keyPair().pk().toUCharVector();

    return scriptSig;
}

}
