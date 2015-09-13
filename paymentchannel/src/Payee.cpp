/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <paymentchannel/Payee.hpp>
#include <common/Payment.hpp>

/**
 * Payee::Status
 */

Payee::Status::Status() {
}

Payee::Status::Status(State state, quint64 numberOfPaymentsMade, quint32 lockTime, quint64 price, const Coin::typesafeOutPoint & contractOutPoint, quint64 funds)
    : _state(state)
    , _numberOfPaymentsMade(numberOfPaymentsMade)
    , _lockTime(lockTime)
    , _price(price)
    , _contractOutPoint(contractOutPoint)
    , _funds(funds) {
}

Payee::State Payee::Status::state() const {
    return _state;
}

void Payee::Status::setState(const Payee::State state) {
    _state = state;
}

quint64 Payee::Status::numberOfPaymentsMade() const {
    return _numberOfPaymentsMade;
}

void Payee::Status::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
    _numberOfPaymentsMade = numberOfPaymentsMade;
}

quint32 Payee::Status::lockTime() const {
    return _lockTime;
}

void Payee::Status::setLockTime(quint32 lockTime) {
    _lockTime = lockTime;
}

quint64 Payee::Status::price() const {
    return _price;
}

void Payee::Status::setPrice(quint64 price) {
    _price = price;
}

Coin::typesafeOutPoint Payee::Status::contractOutPoint() const {
    return _contractOutPoint;
}

void Payee::Status::setContractOutPoint(const Coin::typesafeOutPoint &contractOutPoint) {
    _contractOutPoint = contractOutPoint;
}

quint64 Payee::Status::funds() const {
    return _funds;
}

void Payee::Status::setFunds(quint64 funds) {
    _funds = funds;
}

/**
 * Payee::Configuration
 */

Payee::Configuration::Configuration() {
}

Payee::Configuration::Configuration(State state,
                                    quint64 numberOfPaymentsMade,
                                    const Coin::Signature & lastValidPayorPaymentSignature,
                                    quint32 lockTime,
                                    quint64 price,
                                    quint32 maximumNumberOfSellers,
                                    const Coin::KeyPair & payeeContractKeys,
                                    const Coin::KeyPair & payeePaymentKeys,
                                    const Coin::typesafeOutPoint & contractOutPoint,
                                    const Coin::PublicKey & payorContractPk,
                                    const Coin::PublicKey & payorFinalPk,
                                    quint64 funds)
    : _state(state)
    , _numberOfPaymentsMade(numberOfPaymentsMade)
    , _lastValidPayorPaymentSignature(lastValidPayorPaymentSignature)
    , _lockTime(lockTime)
    , _price(price)
    , _maximumNumberOfSellers(maximumNumberOfSellers)
    , _payeeContractKeys(payeeContractKeys)
    , _payeePaymentKeys(payeePaymentKeys)
    , _contractOutPoint(contractOutPoint)
    , _payorContractPk(payorContractPk)
    , _payorFinalPk(payorFinalPk)
    , _funds(funds) {
}

Payee::State Payee::Configuration::state() const {
    return _state;
}

void Payee::Configuration::setState(State state) {
    _state = state;
}

quint64 Payee::Configuration::numberOfPaymentsMade() const {
    return _numberOfPaymentsMade;
}

void Payee::Configuration::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
    _numberOfPaymentsMade = numberOfPaymentsMade;
}

Coin::Signature Payee::Configuration::lastValidPayorPaymentSignature() const {
    return _lastValidPayorPaymentSignature;
}

void Payee::Configuration::setLastValidPayorPaymentSignature(const Coin::Signature & lastValidPayorPaymentSignature) {
    _lastValidPayorPaymentSignature = lastValidPayorPaymentSignature;
}

quint32 Payee::Configuration::lockTime() const {
    return _lockTime;
}

void Payee::Configuration::setLockTime(quint32 lockTime) {
    _lockTime = lockTime;
}

quint64 Payee::Configuration::price() const {
    return _price;
}

void Payee::Configuration::setPrice(quint64 price) {
    _price = price;
}

quint32 Payee::Configuration::maximumNumberOfSellers() const {
    return _maximumNumberOfSellers;
}

void Payee::Configuration::setMaximumNumberOfSellers(quint32 maximumNumberOfSellers) {
    _maximumNumberOfSellers = maximumNumberOfSellers;
}

Coin::KeyPair Payee::Configuration::payeeContractKeys() const {
    return _payeeContractKeys;
}

void Payee::Configuration::setPayeeContractKeys(const Coin::KeyPair & payeeContractKeys) {
    _payeeContractKeys = payeeContractKeys;
}

Coin::KeyPair Payee::Configuration::payeePaymentKeys() const {
    return _payeePaymentKeys;
}

void Payee::Configuration::setPayeePaymentKeys(const Coin::KeyPair & payeePaymentKeys) {
    _payeePaymentKeys = payeePaymentKeys;
}

Coin::typesafeOutPoint Payee::Configuration::contractOutPoint() const {
    return _contractOutPoint;
}

void Payee::Configuration::setContractOutPoint(const Coin::typesafeOutPoint & contractOutPoint) {
    _contractOutPoint = contractOutPoint;
}

Coin::PublicKey Payee::Configuration::payorContractPk() const {
    return _payorContractPk;
}

void Payee::Configuration::setPayorContractPk(const Coin::PublicKey & payorContractPk) {
    _payorContractPk = payorContractPk;
}

Coin::PublicKey Payee::Configuration::payorFinalPk() const {
    return _payorFinalPk;
}

void Payee::Configuration::setPayorFinalPk(const Coin::PublicKey & payorFinalPk) {
    _payorFinalPk = payorFinalPk;
}

quint64 Payee::Configuration::funds() const {
    return _funds;
}

void Payee::Configuration::setFunds(const quint64 funds) {
    _funds = funds;
}

/**
 * Payee
 */

#include <CoinCore/CoinNodeData.h>
#include <common/P2SHScriptPubKey.hpp>
#include <common/TransactionSignature.hpp>
#include <common/SigHashType.hpp>
#include <paymentchannel/Commitment.hpp>
#include <paymentchannel/Refund.hpp>
#include <paymentchannel/Settlement.hpp>

Payee::Payee() {
}

Payee::Payee(State state,
             quint64 numberOfPaymentsMade,
             const Coin::Signature & lastValidPayorPaymentSignature,
             quint32 lockTime,
             quint64 price,
             quint32 maximumNumberOfSellers,
             const Coin::KeyPair & payeeContractKeys,
             const Coin::KeyPair & payeePaymentKeys,
             const Coin::typesafeOutPoint & contractOutPoint,
             const Coin::PublicKey & payorContractPk,
             const Coin::PublicKey & payorFinalPk,
             quint64 funds)
    : _state(state)
    , _numberOfPaymentsMade(numberOfPaymentsMade)
    , _lastValidPayorPaymentSignature(lastValidPayorPaymentSignature)
    , _lockTime(lockTime)
    , _price(price)
    , _maximumNumberOfSellers(maximumNumberOfSellers)
    , _payeeContractKeys(payeeContractKeys)
    , _payeePaymentKeys(payeePaymentKeys)
    , _contractOutPoint(contractOutPoint)
    , _payorContractPk(payorContractPk)
    , _payorFinalPk(payorFinalPk)
    , _funds(funds) {
}

Payee::Payee(const Payee::Configuration & c)
    : Payee(c.state(),
            c.numberOfPaymentsMade(),
            c.lastValidPayorPaymentSignature(),
            c.lockTime(),
            c.price(),
            c.maximumNumberOfSellers(),
            c.payeeContractKeys(),
            c.payeePaymentKeys(),
            c.contractOutPoint(),
            c.payorContractPk(),
            c.payorFinalPk(),
            c.funds()) {
}

/**
void Payee::registerPayeeInformation(quint32 lockTime, quint32 price, quint32 maximumNumberOfSellers, const KeyPair & payeeContractKeys, const KeyPair & payeePaymentKeys) {

    // Check state
    if(_state != State::waiting_for_payee_information)
        throw std::runtime_error("State incompatible request, must be in waiting_for_payee_information state.");

    _state = State::waiting_for_payor_information;
    _lockTime = lockTime;
    _price = price;
    _payeeContractKeys = payeeContractKeys;
    _payeePaymentKeys = payeePaymentKeys;
}
*/

void Payee::registerPayorInformation(const Coin::typesafeOutPoint & contractOutPoint, const Coin::PublicKey & payorContractPk, const Coin::PublicKey & payorFinalPk, quint64 funds) {

    // Check state
    if(_state != State::waiting_for_payor_information &&
       _state != State::has_all_information_required)
        throw std::runtime_error("State incompatible request, must be in waiting_for_payor_information or has_all_information_required state.");

    _state = State::has_all_information_required;
    _contractOutPoint = contractOutPoint;
    _payorContractPk = payorContractPk;
    _payorFinalPk = payorFinalPk;
    _funds = funds;
}

Commitment Payee::commitment() const {

    if(_state != State::has_all_information_required)
        throw std::runtime_error("State incompatible request, must be has_all_information_required state.");

    return Commitment(_funds,
                      _payorContractPk,
                      _payeeContractKeys.pk());
}

Refund Payee::refund() const {

    if(_state != State::has_all_information_required)
        throw std::runtime_error("State incompatible request, must be has_all_information_required state.");

    return Refund(contractOutPoint(),
                  commitment(),
                  Coin::Payment(_funds, _payorFinalPk.toPubKeyHash()),
                  _lockTime);
}

Settlement Payee::settlement(int64_t paymentCount) const {

    // Setup new payment outputs
    int64_t paid = paymentCount * _price;

    return Settlement(contractOutPoint(),
                      commitment(),
                      Coin::Payment(_funds - paid, _payorFinalPk.toPubKeyHash()),
                      Coin::Payment(paid, _payeePaymentKeys.pk().toPubKeyHash()));
}

Coin::Signature Payee::generateRefundSignature() const {

    // Get refund
    Refund r = refund();

    // Return signature
    return r.transactionSignature(_payeeContractKeys.sk()).sig();
}

bool Payee::registerPayment(const Coin::Signature & paymentSignature) {

    // Get settlement for next payment
    Settlement s = settlement(_numberOfPaymentsMade + 1);

    bool valid = s.validatePayorSignature(paymentSignature);

    // Increase payment count if signature was valid
    if(valid) {
        _numberOfPaymentsMade++;
        _lastValidPayorPaymentSignature = paymentSignature;
    }

    return valid;
}

bool Payee::validateContractTrasaction(const Coin::Transaction & transaction) const {
    return true;
}

Coin::Transaction Payee::lastPaymentTransaction() const {

    // Get settlement for last payment
    Settlement s = settlement(_numberOfPaymentsMade + 1);

    // Prepare signatures
    Coin::TransactionSignature payorTransactionSignature = Coin::TransactionSignature(_lastValidPayorPaymentSignature, Coin::SigHashType::standard());
    Coin::TransactionSignature payeeTransactionSignature = s.transactionSignature(_payeeContractKeys.sk());

    // Return signed transaction
    return s.signedTransaction(payorTransactionSignature, payeeTransactionSignature);
}

Payee::Status Payee::status() const {

    return Status(_state,
                  _numberOfPaymentsMade,
                  _lockTime,
                  _price,
                  _contractOutPoint,
                  _funds);
}

Payee::State Payee::state() const {
    return _state;
}

void Payee::setState(State state) {
    _state = state;
}

quint64 Payee::numberOfPaymentsMade() const {
    return _numberOfPaymentsMade;
}

void Payee::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
    _numberOfPaymentsMade = numberOfPaymentsMade;
}

Coin::Signature Payee::lastValidPayorPaymentSignature() const {
    return _lastValidPayorPaymentSignature;
}

void Payee::setLastValidPayorPaymentSignature(const Coin::Signature & lastValidPayorPaymentSignature) {
    _lastValidPayorPaymentSignature = lastValidPayorPaymentSignature;
}

quint32 Payee::lockTime() const {
    return _lockTime;
}

void Payee::setLockTime(quint32 lockTime) {
    _lockTime = lockTime;
}

quint64 Payee::price() const {
    return _price;
}

void Payee::setPrice(quint64 price) {
    _price = price;
}

quint32 Payee::maximumNumberOfSellers() const {
    return _maximumNumberOfSellers;
}

void Payee::setMaximumNumberOfSellers(quint32 maximumNumberOfSellers) {
    _maximumNumberOfSellers = maximumNumberOfSellers;
}

Coin::KeyPair Payee::payeeContractKeys() const {
    return _payeeContractKeys;
}

void Payee::setPayeeContractKeys(const Coin::KeyPair & payeeContractKeys) {
    _payeeContractKeys = payeeContractKeys;
}

Coin::KeyPair Payee::payeePaymentKeys() const {
    return _payeePaymentKeys;
}

void Payee::setPayeePaymentKeys(const Coin::KeyPair & payeePaymentKeys) {
    _payeePaymentKeys = payeePaymentKeys;
}

Coin::typesafeOutPoint Payee::contractOutPoint() const {
    return _contractOutPoint;
}

void Payee::setContractOutPoint(const Coin::typesafeOutPoint & contractOutPoint) {
    _contractOutPoint = contractOutPoint;
}

Coin::PublicKey Payee::payorContractPk() const {
    return _payorContractPk;
}

void Payee::setPayorContractPk(const Coin::PublicKey & payorContractPk) {
    _payorContractPk = payorContractPk;
}

Coin::PublicKey Payee::payorFinalPk() const {
    return _payorFinalPk;
}

void Payee::setPayorFinalPk(const Coin::PublicKey & payorFinalPk) {
    _payorFinalPk = payorFinalPk;
}

quint64 Payee::funds() const {
    return _funds;
}

void Payee::setFunds(quint64 funds) {
    _funds = funds;
}
