
/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/UnspentP2SHOutput.hpp>
#include <CoinCore/CoinNodeData.h>
#include <CoinCore/secp256k1_openssl.h>
#include <common/Utilities.hpp>

namespace Coin {

UnspentP2SHOutput::UnspentP2SHOutput()
    : UnspentOutput() {
}

UnspentP2SHOutput::UnspentP2SHOutput(const KeyPair & keyPair, const uchar_vector & redeemScript, const uchar_vector & optionalData, const typesafeOutPoint & outPoint, uint64_t setValue)
    : UnspentOutput(outPoint, setValue)
    , _keyPair(keyPair)
    , _redeemScript(redeemScript)
    , _optionalDataLoadingScriptChunk(optionalData) {
}

bool UnspentP2SHOutput::operator==(const UnspentP2SHOutput & o) const {

    return _keyPair == o.keyPair() &&
           _redeemScript == o.redeemScript() &&
           _optionalDataLoadingScriptChunk == o.optionalData() &&
           outPoint() == o.outPoint() &&
           value() == o.value();
}

bool UnspentP2SHOutput::operator!=(const UnspentP2SHOutput & o) const {
    return !(*this == o);
}

KeyPair UnspentP2SHOutput::keyPair() const {
    return _keyPair;
}

void UnspentP2SHOutput::setKeyPair(const KeyPair &keyPair) {
    _keyPair = keyPair;
}

uchar_vector UnspentP2SHOutput::redeemScript() const {
    return _redeemScript;
}

void UnspentP2SHOutput::setRedeemScript(const uchar_vector & script) {
    _redeemScript = script;
}

uchar_vector UnspentP2SHOutput::optionalData() const {
    return _optionalDataLoadingScriptChunk;
}

void UnspentP2SHOutput::setOptionalData(const uchar_vector & optionalData) {
    _optionalDataLoadingScriptChunk = optionalData;
}

uchar_vector UnspentP2SHOutput::scriptPubKey() const {
    return P2SHScriptPubKey::fromSerializedRedeemScript(redeemScript()).serialize();
}

uchar_vector UnspentP2SHOutput::scriptSig(const Transaction & tx, const SigHashType &sigHashType) const {

    //uchar_vector sigHash = sighash(tx, outPoint(), redeemScript(), sigHashType); // TODO update sighash in Utilities
    //TransactionSignature sig(keyPair().sk().sign(sigHash), sigHashType);

    TransactionSignature sig = signTransaction(tx);

    uchar_vector scriptSig;
    scriptSig += sig.opPushForScriptSigSerialized();

    if(optionalData().size() > 0)
        scriptSig += optionalData();

    scriptSig += opPushData(redeemScript().size());
    scriptSig += redeemScript();

    return scriptSig;
}

// Move this to the Utilities
TransactionSignature UnspentP2SHOutput::signTransaction(const Transaction & tx, bool anyoneCanPay) const {
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
                txCopy.inputs[0].scriptSig = redeemScript();
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
                txinput.scriptSig = redeemScript();
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
    return TransactionSignature(keyPair().sk().sign(sigHash), sigHashType);
}

}
