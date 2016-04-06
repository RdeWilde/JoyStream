/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 12 2016
 */

#include <paymentchannel/Channel.hpp>
#include <paymentchannel/Commitment.hpp>
#include <paymentchannel/Refund.hpp>
#include <paymentchannel/Settlement.hpp>
#include <common/RedeemScriptHash.hpp>
#include <common/MultisigScriptPubKey.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/TransactionSignature.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <common/Utilities.hpp> // DEFAULT_SEQUENCE_NUMBER
#include <cassert>
#include <cmath> // ceil

namespace joystream {
namespace paymentchannel {

    Channel::Channel()
        : _price(0)
        , _numberOfPaymentsMade(0)
        , _funds(0)
        , _refundFee(0)
        , _settlementFee(0)
        , _refundLockTime(0) {
    }

    Channel::Channel(quint64 price,
                     quint64 numberOfPaymentsMade,
                     quint64 funds,
                     quint64 refundFee,
                     quint64 settlementFee,
                     quint32 refundLockTime,
                     const Coin::typesafeOutPoint & anchor,
                     const Coin::KeyPair & payorContractKeyPair,
                     const Coin::PubKeyHash & payorFinalPkHash,
                     const Coin::PublicKey & payeeContractPk,
                     const Coin::PubKeyHash & payeeFinalPkHash,
                     const Coin::Signature & payorRefundSignature,
                     const Coin::Signature & payeeRefundSignature)
        : _price(price)
        , _numberOfPaymentsMade(numberOfPaymentsMade)
        , _funds(funds)
        , _refundFee(refundFee)
        , _settlementFee(settlementFee)
        , _refundLockTime(refundLockTime)
        , _anchor(anchor)
        , _payorContractKeyPair(payorContractKeyPair)
        , _payorFinalPkHash(payorFinalPkHash)
        , _payeeContractPk(payeeContractPk)
        , _payeeFinalPkHash(payeeFinalPkHash)
        , _payorRefundSignature(payorRefundSignature)
        , _payeeRefundSignature(payeeRefundSignature) {
    }

    Commitment Channel::commitment() const {

        return Commitment(_funds, _payorContractKeyPair.pk(), _payeeContractPk);
    }

    Refund Channel::refund() const {

        return Refund(_anchor,
                      commitment(),
                      Coin::Payment(_funds - _refundFee, _payorFinalPkHash),
                      _refundLockTime);
    }

    Settlement Channel::settlement() const {

        return Settlement::dustLimitAndFeeAwareSettlement(_anchor,
                                                          commitment(),
                                                          _payorFinalPkHash,
                                                          _payeeFinalPkHash,
                                                          _funds,
                                                          amountPaid(),
                                                          _settlementFee);
    }

    Coin::Signature Channel::generatePayorRefundSignature() const {

        // Get refund
        Refund r = refund();

        // Get refund signature
        Coin::TransactionSignature refundSignature = r.transactionSignature(_payorContractKeyPair.sk());

        return refundSignature.sig();
    }

    Coin::Signature Channel::generatePayorSettlementSignature() const {

        // Get settelemnt
        Settlement s = settlement();

        // Generate signature
        Coin::TransactionSignature settlementSignature = s.transactionSignature(_payorContractKeyPair.sk());

        return settlementSignature.sig();
    }

    Coin::Signature Channel::makePayment() {

        // Increment payment counter
        _numberOfPaymentsMade++;

        // Generate current payment signature
        return generatePayorSettlementSignature();
    }

    bool Channel::checkPayeeRefundSignature(const Coin::Signature & sig) const {

        return refund().validate(_payeeContractPk, sig);
    }

    quint64 Channel::amountPaid() const {
        return _price*_numberOfPaymentsMade;
    }

    Coin::typesafeOutPoint Channel::anchor() const {
        return _anchor;
    }

    void Channel::setAnchor(const Coin::typesafeOutPoint & anchor){

        _anchor = anchor;

        // Generate refund signature for payor
        _payorRefundSignature = generatePayorRefundSignature();
    }

    quint64 Channel::price() const {
        return _price;
    }

    void Channel::setPrice(quint64 price) {
        _price = price;
    }

    quint64 Channel::numberOfPaymentsMade() const {
        return _numberOfPaymentsMade;
    }

    void Channel::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
        _numberOfPaymentsMade = numberOfPaymentsMade;
    }

    quint64 Channel::funds() const {
        return _funds;
    }

    void Channel::setFunds(quint64 funds) {
        _funds = funds;
    }

    quint64 Channel::refundFee() const {
        return _refundFee;
    }

    void Channel::setRefundFee(quint64 refundFee) {
        _refundFee = refundFee;
    }

    quint64 Channel::settlementFee() const {
        return _settlementFee;
    }

    void Channel::setSettlementFee(quint64 settlementFee) {
        _settlementFee = settlementFee;
    }

    quint32 Channel::refundLockTime() const {
        return _refundLockTime;
    }

    void Channel::setRefundLockTime(quint32 refundLockTime) {
        _refundLockTime = refundLockTime;
    }

    Coin::KeyPair Channel::payorContractKeyPair() const {
        return _payorContractKeyPair;
    }

    void Channel::setPayorContractKeyPair(const Coin::KeyPair & payorContractKeyPair) {
        _payorContractKeyPair = payorContractKeyPair;
    }

    Coin::PubKeyHash Channel::payorFinalPkHash() const {
        return _payorFinalPkHash;
    }

    void Channel::setPayorFinalPkHash(const Coin::PubKeyHash & payorFinalPkHash) {
        _payorFinalPkHash = payorFinalPkHash;
    }

    Coin::PublicKey Channel::payeeContractPk() const {
        return _payeeContractPk;
    }

    void Channel::setPayeeContractPk(const Coin::PublicKey & payeeContractPk) {
        _payeeContractPk = payeeContractPk;
    }

    Coin::PubKeyHash Channel::payeeFinalPkHash() const {
        return _payeeFinalPkHash;
    }

    void Channel::setPayeeFinalPkHash(const Coin::PubKeyHash & payeeFinalPkHash) {
        _payeeFinalPkHash = payeeFinalPkHash;
    }

    Coin::Signature Channel::payorRefundSignature() const {
        return _payorRefundSignature;
    }

    void Channel::setPayorRefundSignature(const Coin::Signature & payorRefundSignature) {
        _payorRefundSignature = payorRefundSignature;
    }

    Coin::Signature Channel::payeeRefundSignature() const {
        return _payeeRefundSignature;
    }

    void Channel::setPayeeRefundSignature(const Coin::Signature & payeeRefundSignature) {
        _payeeRefundSignature = payeeRefundSignature;
    }
        
    quint64 requiredFee(int numberOfPayees, quint64 feePerKb) {
        
        // One output for change, and one per payee
        int numberOfOutputs = numberOfPayees + 1;
        
        // Sizevof transaction
        quint64 txByteSize =(148*1) + (34*(numberOfPayees + 1) + 10);

        // Seed on fee estimate at http://bitcoinfees.com/
        return ceil(feePerKb*((float)txByteSize/1000));

    }

    //std::vector<TxIn>

    Coin::Transaction contractTransaction(const Coin::UnspentP2PKHOutput & funding,
                                          const std::vector<Commitment> & commitments,
                                          const Coin::Payment & change) {

        // Create transaction
        Coin::Transaction transaction;

        // Iterate channels and generate outputs
        for(std::vector<Commitment>::const_iterator i = commitments.cbegin(); i != commitments.cend();i++)
            transaction.addOutput((*i).contractOutput());

        // Add change output
        transaction.addOutput(change.txOut());

        // NB use higher level signing routines coming soon!

        // Add (unsigned) input spending funding utxo
        transaction.addInput(Coin::TxIn(funding.outPoint().getClassicOutPoint(), uchar_vector(), DEFAULT_SEQUENCE_NUMBER));

        // Creates spending input script
        setScriptSigToSpendP2PKH(transaction, 0, funding.keyPair().sk());

        return transaction;
    }

    Coin::Transaction anchor(const Coin::UnspentP2PKHOutput & funding,
                             std::vector<Channel> & channels,
                             const Coin::Payment & change) {

        // Extract commitments for channels
        std::vector<Commitment> commitments;

        for(std::vector<Channel>::const_iterator i = channels.cbegin(); i != channels.cend();i++)
            commitments.push_back((*i).commitment());

        assert(channels.size() == commitments.size());

        // Create contract transaction
        Coin::Transaction tx = contractTransaction(funding, commitments, change);

        // Get contract transaction id
        Coin::TransactionId txId = Coin::TransactionId::fromTx(tx);

        // Iterate outputs and anchor corresponding channel
        assert(tx.outputs.size() == commitments.size() + 1); // +1 due to change output

        for(int i = 0;i < commitments.size();i++) {

            // Create outpoint
            Coin::typesafeOutPoint o(txId, i);

            // Anchor channel
            channels[i].setAnchor(o);
        }

        return tx;
    }
}
}
