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
        , _settlementFee(0)
        , _refundLockTime(0) {
    }

    Payor::Payor(quint64 price,
                     quint64 numberOfPaymentsMade,
                     quint64 funds,
                     quint64 settlementFee,
                     quint32 refundLockTime,
                     const Coin::typesafeOutPoint & anchor,
                     const Coin::KeyPair & payorContractKeyPair,
                     const Coin::RedeemScriptHash &payorFinalScriptHash,
                     const Coin::PublicKey & payeeContractPk,
                     const Coin::RedeemScriptHash &payeeFinalScriptHash,
                     const Coin::Signature & payorRefundSignature,
                     const Coin::Signature & payeeRefundSignature)
        : _price(price)
        , _numberOfPaymentsMade(numberOfPaymentsMade)
        , _funds(funds)
        , _settlementFee(settlementFee)
        , _refundLockTime(refundLockTime)
        , _anchor(anchor)
        , _payorContractKeyPair(payorContractKeyPair)
        , _payorFinalScriptHash(payorFinalScriptHash)
        , _payeeContractPk(payeeContractPk)
        , _payeeFinalScriptHash(payeeFinalScriptHash){
    }

    Commitment Payor::commitment() const {

        return Commitment(_funds, _payorContractKeyPair.pk(), _payeeContractPk, _refundLockTime);
    }

    Refund Payor::refund() const {

        //return Refund...
    }

    Settlement Payor::settlement() const {

        return Settlement::dustLimitAndFeeAwareSettlement(_anchor,
                                                          commitment(),
                                                          _payorFinalScriptHash,
                                                          _payeeFinalScriptHash,
                                                          _funds,
                                                          amountPaid(),
                                                          _settlementFee);
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

    quint64 Payor::amountPaid() const {
        return _price*_numberOfPaymentsMade;
    }

    Coin::typesafeOutPoint Payor::anchor() const {
        return _anchor;
    }

    void Payor::setAnchor(const Coin::typesafeOutPoint & anchor){

        _anchor = anchor;

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

    Coin::RedeemScriptHash Payor::payorFinalScriptHash() const {
        return _payorFinalScriptHash;
    }

    void Payor::setPayorFinalScriptHash(const Coin::RedeemScriptHash & payorFinalScriptHash) {
        _payorFinalScriptHash = payorFinalScriptHash;
    }

    Coin::PublicKey Payor::payeeContractPk() const {
        return _payeeContractPk;
    }

    void Payor::setPayeeContractPk(const Coin::PublicKey & payeeContractPk) {
        _payeeContractPk = payeeContractPk;
    }

    Coin::RedeemScriptHash Payor::payeeFinalScriptHash() const {
        return _payeeFinalScriptHash;
    }

    void Payor::setPayeeFinalScriptHash(const Coin::RedeemScriptHash & payeeFinalScriptHash) {
        _payeeFinalScriptHash = payeeFinalScriptHash;
    }

}
}
