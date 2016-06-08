/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 12 2016
 */

#include <paymentchannel/Payor.hpp>
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

    Payor::Payor()
        : _price(0)
        , _numberOfPaymentsMade(0)
        , _funds(0)
        , _refundFee(0)
        , _settlementFee(0)
        , _refundLockTime(0) {
    }

    Payor::Payor(quint64 price,
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

    Commitment Payor::commitment() const {

        return Commitment(_funds, _payorContractKeyPair.pk(), _payeeContractPk, _refundLockTime);
    }

    Refund Payor::refund() const {

        return Refund(_anchor,
                      commitment(),
                      Coin::Payment(_funds - _refundFee, _payorFinalPkHash),
                      _refundLockTime);
    }

    Settlement Payor::settlement() const {

        return Settlement::dustLimitAndFeeAwareSettlement(_anchor,
                                                          commitment(),
                                                          _payorFinalPkHash,
                                                          _payeeFinalPkHash,
                                                          _funds,
                                                          amountPaid(),
                                                          _settlementFee);
    }

    Coin::Signature Payor::generatePayorRefundSignature() const {

        // Get refund
        Refund r = refund();

        // Get refund signature
        Coin::TransactionSignature refundSignature = r.transactionSignature(_payorContractKeyPair.sk());

        return refundSignature.sig();
    }

    Coin::Signature Payor::generatePayorSettlementSignature() const {

        // Get settelemnt
        Settlement s = settlement();

        // Generate signature
        Coin::TransactionSignature settlementSignature = s.transactionSignature(_payorContractKeyPair.sk());

        return settlementSignature.sig();
    }

    Coin::Signature Payor::makePayment() {

        // Increment payment counter
        _numberOfPaymentsMade++;

        // Generate current payment signature
        return generatePayorSettlementSignature();
    }

    bool Payor::checkPayeeRefundSignature(const Coin::Signature & sig) const {

        return refund().validate(_payeeContractPk, sig);
    }

    quint64 Payor::amountPaid() const {
        return _price*_numberOfPaymentsMade;
    }

    Coin::typesafeOutPoint Payor::anchor() const {
        return _anchor;
    }

    void Payor::setAnchor(const Coin::typesafeOutPoint & anchor){

        _anchor = anchor;

        // Generate refund signature for payor
        _payorRefundSignature = generatePayorRefundSignature();
    }

    quint64 Payor::price() const {
        return _price;
    }

    void Payor::setPrice(quint64 price) {
        _price = price;
    }

    quint64 Payor::numberOfPaymentsMade() const {
        return _numberOfPaymentsMade;
    }

    void Payor::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
        _numberOfPaymentsMade = numberOfPaymentsMade;
    }

    quint64 Payor::funds() const {
        return _funds;
    }

    void Payor::setFunds(quint64 funds) {
        _funds = funds;
    }

    quint64 Payor::refundFee() const {
        return _refundFee;
    }

    void Payor::setRefundFee(quint64 refundFee) {
        _refundFee = refundFee;
    }

    quint64 Payor::settlementFee() const {
        return _settlementFee;
    }

    void Payor::setSettlementFee(quint64 settlementFee) {
        _settlementFee = settlementFee;
    }

    quint32 Payor::refundLockTime() const {
        return _refundLockTime;
    }

    void Payor::setRefundLockTime(quint32 refundLockTime) {
        _refundLockTime = refundLockTime;
    }

    Coin::KeyPair Payor::payorContractKeyPair() const {
        return _payorContractKeyPair;
    }

    void Payor::setPayorContractKeyPair(const Coin::KeyPair & payorContractKeyPair) {
        _payorContractKeyPair = payorContractKeyPair;
    }

    Coin::PubKeyHash Payor::payorFinalPkHash() const {
        return _payorFinalPkHash;
    }

    void Payor::setPayorFinalPkHash(const Coin::PubKeyHash & payorFinalPkHash) {
        _payorFinalPkHash = payorFinalPkHash;
    }

    Coin::PublicKey Payor::payeeContractPk() const {
        return _payeeContractPk;
    }

    void Payor::setPayeeContractPk(const Coin::PublicKey & payeeContractPk) {
        _payeeContractPk = payeeContractPk;
    }

    Coin::PubKeyHash Payor::payeeFinalPkHash() const {
        return _payeeFinalPkHash;
    }

    void Payor::setPayeeFinalPkHash(const Coin::PubKeyHash & payeeFinalPkHash) {
        _payeeFinalPkHash = payeeFinalPkHash;
    }

    Coin::Signature Payor::payorRefundSignature() const {
        return _payorRefundSignature;
    }

    void Payor::setPayorRefundSignature(const Coin::Signature & payorRefundSignature) {
        _payorRefundSignature = payorRefundSignature;
    }

    Coin::Signature Payor::payeeRefundSignature() const {
        return _payeeRefundSignature;
    }

    void Payor::setPayeeRefundSignature(const Coin::Signature & payeeRefundSignature) {
        _payeeRefundSignature = payeeRefundSignature;
    }
}
}
