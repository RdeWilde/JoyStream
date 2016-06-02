/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/UnspentOutput.hpp>
#include <CoinCore/CoinNodeData.h>
#include <CoinCore/secp256k1_openssl.h>

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

TransactionSignature UnspentOutput::signTransaction(const Transaction &tx, bool anyoneCanPay) const {

    SigHashType sigHashType(SigHashType::MutuallyExclusiveType::all, anyoneCanPay);

    // Make a copy of the transaction
    Coin::Transaction txCopy = tx;

    // Sign only the input which spends this output
    // https://en.bitcoin.it/wiki/OP_CHECKSIG#Procedure_for_Hashtype_SIGHASH_ANYONECANPAY
    if(anyoneCanPay) {
        // Remove all inputs
        txCopy.inputs.clear();

        for(auto & txinput : tx.inputs) {
            if(typesafeOutPoint(txinput.previousOut) == outPoint()) {
                // This is the input to sign
                txCopy.inputs.push_back(txinput);
                txCopy.inputs[0].scriptSig = scriptPubKey();
                break;
            }
        }

        if(txCopy.inputs.size() != 1) {
            throw std::runtime_error("Transaction does not have a corresponding input");
        }

    } else {
        // Sign all inputs

        bool foundInput = false;

        // Clear input signatures
        for(auto & txinput : txCopy.inputs) {
            if(typesafeOutPoint(txinput.previousOut) == outPoint()) {
                txinput.scriptSig = scriptPubKey();
                foundInput = true;
            } else {
                txinput.scriptSig.clear();
            }
        }

        if(!foundInput) {
            throw std::runtime_error("Transaction does not have a corresponding input");
        }
    }

    // Compute sighash
    uchar_vector sigHash = txCopy.getHashWithAppendedCode(sigHashType.hashCode());

    // Create signature
    return TransactionSignature(uchar_vector(sign(sigHash)), sigHashType);
}

bytes_t UnspentOutput::sign(const bytes_t &data) const {
    CoinCrypto::secp256k1_key signingKey;
    signingKey.setPrivKey(keyPair().sk().toUCharVector());

    // Comute signature and return
    return CoinCrypto::secp256k1_sign(signingKey, data);
}

}
