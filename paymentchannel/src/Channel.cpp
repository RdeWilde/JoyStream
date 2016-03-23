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
#include <common/Utilities.hpp> // DEFAULT_SEQUENCE_NUMBER
#include <cassert>

namespace joystream {
namespace paymentchannel {

    Channel::Channel() {
    }

    Channel::Channel(quint64 price,
                     quint64 numberOfPaymentsMade,
                     quint64 funds,
                     quint64 refundFee,
                     quint64 settlementFee,
                     quint32 refundLockTime,
                     const Coin::typesafeOutPoint & anchor,
                     const Coin::KeyPair & payorContractKeyPair,
                     const Coin::PubKeyHash & payorFinalKeyHash,
                     const Coin::PublicKey & payeeContractPk,
                     const Coin::PubKeyHash & payeeFinalKeyHash,
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
        , _payorFinalKeyHash(payorFinalKeyHash)
        , _payeeContractPk(payeeContractPk)
        , _payeeFinalKeyHash(payeeFinalKeyHash)
        , _payorRefundSignature(payorRefundSignature)
        , _payeeRefundSignature(payeeRefundSignature) {
    }

    Commitment Channel::commitment() const {

        return Commitment(_funds, _payorContractKeyPair.pk(), _payeeContractPk);
    }

    Refund Channel::refund() const {

        return Refund(_anchor,
                      commitment(),
                      Coin::Payment(_funds - _refundFee, _payorFinalKeyHash),
                      _refundLockTime);
    }

    Settlement Channel::settlement() const {

        return Settlement::dustLimitAndFeeAwareSettlement(_anchor,
                                                          commitment(),
                                                          _payorFinalKeyHash,
                                                          _payeeFinalKeyHash,
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

    Coin::KeyPair Channel::payorContractKeyPair() const {
        return _payorContractKeyPair;
    }

    void Channel::setPayorContractKeyPair(const Coin::KeyPair & payorContractKeyPair) {
        _payorContractKeyPair = payorContractKeyPair;
    }

    Coin::PubKeyHash Channel::payorFinalKeyHash() const {
        return _payorFinalKeyHash;
    }

    void Channel::setPayorFinalKeyHash(const Coin::PubKeyHash & payorFinalKeyHash) {
        _payorFinalKeyHash = payorFinalKeyHash;
    }

    Coin::PublicKey Channel::payeeContractPk() const {
        return _payeeContractPk;
    }

    void Channel::setPayeeContractPk(const Coin::PublicKey & payeeContractPk) {
        _payeeContractPk = payeeContractPk;
    }

    Coin::PubKeyHash Channel::payeeFinalKeyHash() const {
        return _payeeFinalKeyHash;
    }

    void Channel::setPayeeFinalKeyHash(const Coin::PubKeyHash & payeeFinalKeyHash) {
        _payeeFinalKeyHash = payeeFinalKeyHash;
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
}
}
