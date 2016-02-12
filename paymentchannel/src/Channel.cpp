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

//#include <QtMath>
//#include <QDebug>

namespace joystream {
namespace paymentchannel {

    Channel::Channel() {
    }

    Channel::Channel(quint32 index,
                    ChannelState state,
                    quint64 price,
                    quint64 numberOfPaymentsMade,
                    quint64 funds,
                    const Coin::KeyPair & payorContractKeyPair,
                    const Coin::KeyPair & payorFinalKeyPair,
                    const Coin::PublicKey & payeeContractPk,
                    const Coin::PublicKey & payeeFinalPk,
                    const Coin::Signature & payorRefundSignature,
                    const Coin::Signature & payeeRefundSignature,
                    quint64 refundFee,
                    quint64 paymentFee,
                    quint32 refundLockTime)
        : _index(index)
        , _state(state)
        , _price(price)
        , _numberOfPaymentsMade(numberOfPaymentsMade)
        , _funds(funds)
        , _payorContractKeyPair(payorContractKeyPair)
        , _payorFinalKeyPair(payorFinalKeyPair)
        , _payeeContractPk(payeeContractPk)
        , _payeeFinalPk(payeeFinalPk)
        , _payorRefundSignature(payorRefundSignature)
        , _payeeRefundSignature(payeeRefundSignature)
        , _refundFee(refundFee)
        , _paymentFee(paymentFee)
        , _refundLockTime(refundLockTime) {
    }

    Coin::typesafeOutPoint Channel::contractOutPoint(const Coin::TransactionId & contractTxId) const {
        return Coin::typesafeOutPoint(contractTxId, _index);
    }

    Commitment Channel::commitment() const {

        if(_state == ChannelState::unassigned)
            throw std::runtime_error("No valid commitment exists for channel.");

        return Commitment(_funds, _payorContractKeyPair.pk(), _payeeContractPk);
    }

    Refund Channel::refund(const Coin::TransactionId & contractTxId) const {

        if(_state == ChannelState::unassigned)
            throw std::runtime_error("No valid commitment exists for channel.");

        // *** no fee !!!
        return Refund(contractOutPoint(contractTxId),
                      commitment(),
                      Coin::Payment(_funds, _payorFinalKeyPair.pk().toPubKeyHash()),
                      _refundLockTime);
    }

    Settlement Channel::settlement(const Coin::TransactionId & contractTxId, quint64 paychanSettlementFee) const {

        if(_state == ChannelState::unassigned)
            throw std::runtime_error("No valid commitment exists for channel.");

        // The amount paid so far
        quint64 paid = _price*_numberOfPaymentsMade;

        Q_ASSERT(paid <= _funds);

        return Settlement::dustLimitAndFeeAwareSettlement(contractOutPoint(contractTxId),
                                                          commitment(),
                                                          _payorFinalKeyPair.pk().toPubKeyHash(),
                                                          _payeeFinalPk.toPubKeyHash(),
                                                          _funds,
                                                          paid,
                                                          paychanSettlementFee);
    }

    /**
    Coin::Signature Channel::createPayorRefundSignature(const Coin::TransactionId & contractHash) const {

        // Check that channel has been assigned
        if(_state != State::assigned)
            throw std::runtime_error("State incompatile request, must be in assigned state.");

        // Create transaction for spening contract output
        Coin::Transaction refund = contractSpendingTransaction(contractHash, _funds, _refundLockTime);

        // Return signature
        return ts.sig();
    }


    Coin::Signature Channel::createPaymentSignature(const Coin::TransactionId & contractHash) const {

        // Check that channel has been assigned
        if(_state != State::assigned)
            throw std::runtime_error("State incompatile request, must be in assigned state.");

        // How much has been paid to payee
        quint64 paid = _numberOfPaymentsMade*_price;

        // Create transaction for spening contract output
        Coin::Transaction payment = contractSpendingTransaction(contractHash, _funds - paid);

        // Add payee output
        payment.addOutput(Coin::TxOut(paid, Coin::P2PKHScriptPubKey(_payeeFinalPk).serialize()));

        // Create payor signature
        Coin::TransactionSignature ts = _payorContractKeyPair.sk().sign(payment, 0, contractOutputScriptPubKey().serialize(), Coin::SigHashType::all);

        // Return signature
        return ts.sig();
    }

    bool Channel::validateRefundSignature(const Coin::TransactionId & contractHash, const Coin::Signature & payeeSig) const {

        // Check that channel has been assigned
        if(_state != State::assigned)
            throw std::runtime_error("State incompatible request, must be in assigned state.");

        // Create transaction for spending contract output
        Coin::Transaction refund = contractSpendingTransaction(contractHash, _funds);

        // Compute sighash
        uchar_vector sigHash = Coin::sighash(refund,
                                             0,
                                             contractOutputScriptPubKey().serialize(),
                                             Coin::SigHashType::all);

        // Verify that signature is valid for payee public key on sighash
        return _payeeContractPk.verify(sigHash, payeeSig);
    }
    */

    void Channel::increaseNumberOfPayments() {
        _numberOfPaymentsMade++;
    }

    quint32 Channel::index() const {
        return _index;
    }

    void Channel::setIndex(quint32 index) {
        _index = index;
    }

    ChannelState Channel::state() const {
        return _state;
    }

    void Channel::setState(ChannelState state) {
        _state = state;
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

    Coin::KeyPair Channel::payorContractKeyPair() const {
        return _payorContractKeyPair;
    }

    void Channel::setPayorContractKeyPair(const Coin::KeyPair & payorContractKeyPair) {
        _payorContractKeyPair = payorContractKeyPair;
    }

    Coin::KeyPair Channel::payorFinalKeyPair() const {
        return _payorFinalKeyPair;
    }

    void Channel::setPayorFinalKeyPair(const Coin::KeyPair & payorFinalKeyPair) {
        _payorFinalKeyPair = payorFinalKeyPair;
    }

    Coin::PublicKey Channel::payeeContractPk() const {
        return _payeeContractPk;
    }

    void Channel::setPayeeContractPk(const Coin::PublicKey & payeeContractPk) {
        _payeeContractPk = payeeContractPk;
    }

    Coin::PublicKey Channel::payeeFinalPk() const {
        return _payeeFinalPk;
    }

    void Channel::setPayeeFinalPk(const Coin::PublicKey & payeeFinalPk) {
        _payeeFinalPk = payeeFinalPk;
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

    quint64 Channel::refundFee() const {
        return _refundFee;
    }

    void Channel::setRefundFee(quint64 refundFee) {
        _refundFee = refundFee;
    }

    quint64 Channel::paymentFee() const {
        return _paymentFee;
    }

    void Channel::setPaymentFee(quint64 paymentFee) {
        _paymentFee = paymentFee;
    }

    quint32 Channel::refundLockTime() const {
        return _refundLockTime;
    }

    void Channel::setRefundLockTime(quint32 refundLockTime) {
        _refundLockTime = refundLockTime;
    }


}
}
