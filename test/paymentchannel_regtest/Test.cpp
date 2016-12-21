/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <Test.hpp>

#include "bitcoin_regtest_framework.hpp"

#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/P2PKHAddress.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <common/UnspentOutputSet.hpp>
#include <common/Utilities.hpp>

#include <paymentchannel/paymentchannel.hpp>

#include <CoinCore/CoinNodeData.h> // Coin::Transaction

using namespace joystream::test::bitcoin;
using namespace joystream::paymentchannel;

void Test::initTestCase() {

    QVERIFY(regtest::init() == 0);
}

void Test::init() {

}

void Test::cleanup() {

}

void Test::cleanupTestCase() {
    // Stop bitcoind
    regtest::shutdown();
}

Coin::typesafeOutPoint getOutpointFromAddress(std::string &txid, Coin::P2PKHAddress &address) {
    std::string rawTx = regtest::getrawtransaction(txid);
    auto scriptPubKey = Coin::P2PKHScriptPubKey(address.pubKeyHash()).serialize();
    Coin::Transaction tx(rawTx);

    for(uint i = 0; i < tx.outputs.size(); i++) {
        if(tx.outputs.at(i).scriptPubKey == scriptPubKey) {
            return Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(txid), i);
        }
    }

    return Coin::typesafeOutPoint();
}

Coin::UnspentOutputSet getFunds(int64_t amount) {
    std::stringstream amount_ss;
    amount_ss << (float)amount / 100000000;
    Coin::PrivateKey sk = Coin::PrivateKey::generate();
    Coin::P2PKHAddress address(Coin::Network::regtest, sk.toPublicKey().toPubKeyHash());

    std::string fundsTxId;
    if(regtest::send_to_address(address.toBase58CheckEncoding().toStdString(), amount_ss.str(), fundsTxId) != 0) {
        return Coin::UnspentOutputSet();
    }

    Coin::typesafeOutPoint outpoint = getOutpointFromAddress(fundsTxId, address);

    Coin::UnspentOutputSet funding;
    auto funds = new Coin::UnspentP2PKHOutput(Coin::KeyPair(sk), outpoint, amount);
    funding.insert(funding.end(), std::shared_ptr<Coin::UnspentOutput>(funds));
    return funding;
}

void Test::RefundLocking() {
    const int64_t funds = 500000;
    const int64_t contractFee = 1000;
    const int64_t refundFee = 1000;
    const int64_t commitmentAmount = funds - contractFee;
    const int64_t refundAmount = commitmentAmount - refundFee;

    auto funding = getFunds(funds);
    QCOMPARE(funding.size(), (size_t)1);

    // Generate Keys for the payment channel commitment
    auto buyerSk = Coin::PrivateKey::generate();
    auto sellerSk = Coin::PrivateKey::generate();

    // For easy regtesting we will use locktime in block units
    const uint lockTimeBlocks = 20;
    Coin::RelativeLockTime locktime(Coin::RelativeLockTime::Units::Blocks, lockTimeBlocks);

    // Construct a Contract with one commitment
    ContractTransactionBuilder contract;
    contract.setFunding(funding);

    Commitment commitment(commitmentAmount, buyerSk.toPublicKey(), sellerSk.toPublicKey(), locktime);

    contract.setCommitments(ContractTransactionBuilder::Commitments({commitment}));

    auto contractTx = contract.transaction();

    //std::cout << "bitcore::scriptPubKey::" << contractTx.outputs.at(0).scriptPubKey.getHex() << std::endl;

    std::string rawContractTransaction = contractTx.getSerialized().getHex();

    // Broadcast the contract
    QCOMPARE(regtest::send_raw_transaction(rawContractTransaction), 0);

    // Construct a buyer full Refund
    auto contractTxId = Coin::TransactionId::fromTx(contractTx);
    Refund refund(Coin::typesafeOutPoint(contractTxId, 0), commitment, Coin::KeyPair(buyerSk));

    // Get an unspent output to use to finance our refund transaction
    auto refundUnspentOutput = refund.getUnspentOutput();
    Coin::UnspentOutputSet refundUnspentOutputSet;
    refundUnspentOutputSet.insert(refundUnspentOutputSet.end(), std::shared_ptr<Coin::UnspentOutput>(refundUnspentOutput));

    Coin::Transaction refundTx;

    // To indicate that we accept new consensus rules (BIP68 - Relative Lock Time, we must set the transaction
    // version to be greater than 1
    refundTx.version = 2;

    // Send the refund to a bitcoind wallet controlled address
    auto refundDestination = Coin::P2PKHAddress::fromBase58CheckEncoding(QString::fromStdString(regtest::getnewaddress()));
    auto refundOutput = Coin::TxOut(refundAmount, Coin::P2PKHScriptPubKey(refundDestination.pubKeyHash()).serialize());
    refundTx.addOutput(refundOutput);

    // finance the transaction
    refundUnspentOutputSet.finance(refundTx, Coin::SigHashType::standard());

    // Check that our tx was properly financed
    QCOMPARE(refundTx.inputs.size(), (size_t)1);

    // ... and the sequence number has been set correctly
    QCOMPARE(refundTx.inputs.at(0).sequence, locktime.toSequenceNumber());

    std::string rawRefundTransaction = refundTx.getSerialized().getHex();

    //std::cout << "bitcore::scriptSig::" << refundTx.inputs.at(0).scriptSig.getHex() << std::endl;
    //std::cout << "Transaction:" << refundTx.toJson() << std::endl;
    //std::cout << "bitcore::tx:" << rawRefundTransaction << std::endl;

    // Trying to spend the refund Tx before the locktime expires.. should fail
    QCOMPARE(regtest::send_raw_transaction(rawRefundTransaction) == 0, false);

    // Generate enough blocks for the locktime to expire
    QCOMPARE(regtest::generate_blocks(lockTimeBlocks+1), 0);

    // Trying to spend the refund after the locktime expires should work
    QCOMPARE(regtest::send_raw_transaction(rawRefundTransaction) == 0, true);
}

void Test::Settlement() {
    const int64_t funds = 500000;
    const int64_t contractFee = 5000;
    const int64_t commitmentAmount = funds - contractFee;
    const int64_t paymentAmount = 200000;
    const int64_t settlementFee = 5000;

    auto funding = getFunds(funds);
    QCOMPARE(funding.size(), (size_t)1);

    // Generate Keys for the payment channel commitment
    auto buyerSk = Coin::PrivateKey::generate();
    auto sellerSk = Coin::PrivateKey::generate();

    // For easy regtesting we will use locktime in block units
    const uint lockTimeBlocks = 20;
    Coin::RelativeLockTime locktime(Coin::RelativeLockTime::Units::Blocks, lockTimeBlocks);

    // Construct a Contract with one commitment
    ContractTransactionBuilder contract;

    contract.setFunding(funding);

    Commitment commitment(commitmentAmount, buyerSk.toPublicKey(), sellerSk.toPublicKey(), locktime);

    contract.setCommitments(ContractTransactionBuilder::Commitments({commitment}));

    auto contractTx = contract.transaction();

    //std::cout << "bitcore::scriptPubKey::" << contractTx.outputs.at(0).scriptPubKey.getHex() << std::endl;

    std::string rawContractTransaction = contractTx.getSerialized().getHex();

    // Broadcast the contract
    QCOMPARE(regtest::send_raw_transaction(rawContractTransaction), 0);

    Coin::KeyPair payorFinalPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeFinalPair = Coin::KeyPair::generate();

    Coin::Payment toPayor(commitmentAmount - settlementFee - paymentAmount, payorFinalPair.pk().toP2PKHAddress(Coin::Network::regtest));
    Coin::Payment toPayee(paymentAmount, payeeFinalPair.pk().toP2PKHAddress(Coin::Network::regtest));

    auto contractTxId = Coin::TransactionId::fromTx(contractTx);

    joystream::paymentchannel::Settlement s(Coin::typesafeOutPoint(contractTxId, 0),
                                            commitment,
                                            toPayor,
                                            toPayee);
    // Generate payee refund signature, hence using payee private key
    Coin::TransactionSignature payeePaySig = s.transactionSignature(sellerSk);
    Coin::TransactionSignature payorPaySig = s.transactionSignature(buyerSk);

    bool validPayeeSettlementSig = s.validatePayeeSignature(payeePaySig.sig());
    QVERIFY(validPayeeSettlementSig);

    bool validPayorSettlementSig = s.validatePayorSignature(payorPaySig.sig());
    QVERIFY(validPayorSettlementSig);

    auto settlementTx = s.signedTransaction(payorPaySig, payeePaySig);

    std::string rawSettlementTransaction = settlementTx.getSerialized().getHex();

    // Broadcast the contract
    QCOMPARE(regtest::send_raw_transaction(rawSettlementTransaction), 0);
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
