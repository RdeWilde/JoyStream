/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 5 2015
 */

#include <paymentchannel/Termination.hpp>
#include <common/Utilities.hpp> // DEFAULT_SEQUENCE_NUMBER, sighash, sign
#include <common/SigHashType.hpp>
#include <common/TransactionSignature.hpp>
#include <common/P2SHScriptSig.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/MultisigScriptPubKey.hpp>
#include <common/PrivateKey.hpp>

Termination::Termination(const Coin::typesafeOutPoint & contractOutPoint,
                         const Commitment & commitment,
                         const Coin::Payment & toPayor)
    : _contractOutPoint(contractOutPoint)
    , _commitment(commitment)
    , _toPayor(toPayor) {
}

Coin::Transaction Termination::unSignedTransaction() const {

    // Build refund transaction
    Coin::Transaction tx;

    // Add payor refund output
    tx.addOutput(_toPayor.txOut());

    // Add (unsigned) input spending contract output
    tx.addInput(Coin::TxIn(_contractOutPoint.getClassicOutPoint(), uchar_vector(), DEFAULT_SEQUENCE_NUMBER));

    return tx;
}

Coin::TransactionSignature Termination::transactionSignature(const Coin::PrivateKey & sk) const {
    return sk.sign(unSignedTransaction(), 0, _commitment.contractOutputScriptPubKey().serialize(), Coin::SigHashType::standard());
}

Coin::Transaction Termination::signedTransaction(const Coin::TransactionSignature & payorTransactionSignature,
                                                 const Coin::TransactionSignature & payeeTransactionSignature) const {

    // Verify that both have appropriate sighash type
    if(!payorTransactionSignature.type().isStandard())
        throw std::runtime_error("First signature was not of type SIGHASH_ALL type");
    else if(!payeeTransactionSignature.type().isStandard())
        throw std::runtime_error("Second signature was not of type SIGHASH_ALL type");

    // Create (unsigned) transaction
    Coin::Transaction tx = unSignedTransaction();

    // Create input script with signature
    Coin::P2SHScriptSig scriptSig(std::vector<Coin::TransactionSignature>({payorTransactionSignature, payeeTransactionSignature}),
                                  _commitment.terminationScriptSig().serialized());

    tx.inputs[0].scriptSig = scriptSig.serialized();

    return tx;
}

bool Termination::validate(const Coin::PublicKey & pk, const Coin::Signature & sig) const {

    // Compute sighash for unsigned refund
    uchar_vector sigHash = sighash(unSignedTransaction(),
                                   0,
                                   _commitment.contractOutputScriptPubKey().serialize(),
                                   Coin::SigHashType::standard());

    // Check if signature is valid
    return pk.verify(sigHash, sig);
}

bool Termination::validatePayorSignature(const Coin::Signature & sig) const {
    return validate(_commitment.firstPk(), sig);
}

bool Termination::validatePayeeSignature(const Coin::Signature & sig) const {
    return validate(_commitment.secondPk(), sig);
}
