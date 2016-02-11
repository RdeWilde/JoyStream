/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <paymentchannel/Payee.hpp>
#include <paymentchannel/PayeeConfiguration.hpp>
#include <paymentchannel/Commitment.hpp>
#include <paymentchannel/Refund.hpp>
#include <paymentchannel/Settlement.hpp>

#include <common/P2SHScriptPubKey.hpp>
#include <common/TransactionSignature.hpp>
#include <common/SigHashType.hpp>
#include <common/Bitcoin.hpp> // TEMPORARY, JUST TO GET HARD CODED SETTLEMENT FEE
#include <common/Payment.hpp>

#include <CoinCore/CoinNodeData.h>

namespace joystream {
namespace paymentchannel {

    Payee::Payee() {
    }

    Payee::Payee(PayeeState state,
                 quint64 numberOfPaymentsMade,
                 const Coin::Signature & lastValidPayorPaymentSignature,
                 quint32 lockTime,
                 quint64 price,
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
        , _payeeContractKeys(payeeContractKeys)
        , _payeePaymentKeys(payeePaymentKeys)
        , _contractOutPoint(contractOutPoint)
        , _payorContractPk(payorContractPk)
        , _payorFinalPk(payorFinalPk)
        , _funds(funds) {
    }

    Payee::Payee(const PayeeConfiguration & c)
        : Payee(c.state(),
                c.numberOfPaymentsMade(),
                c.lastValidPayorPaymentSignature(),
                c.lockTime(),
                c.price(),
                c.payeeContractKeys(),
                c.payeePaymentKeys(),
                c.contractOutPoint(),
                c.payorContractPk(),
                c.payorFinalPk(),
                c.funds()) {
    }

    Payee Payee::initPayeeWithoutKnownPayor(quint32 lockTime,
                       quint64 price,
                       const Coin::KeyPair & payeeContractKeys,
                       const Coin::KeyPair & payeePaymentKeys) {

        return Payee(PayeeState::waiting_for_payor_information,
                     0,
                     Coin::Signature(),
                     lockTime,
                     price,
                     payeeContractKeys,
                     payeePaymentKeys,
                     Coin::typesafeOutPoint(),
                     Coin::PublicKey(),
                     Coin::PublicKey(),
                     0);
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
        if(_state != PayeeState::waiting_for_payor_information &&
           _state != PayeeState::has_all_information_required)
            throw std::runtime_error("State incompatible request, must be in waiting_for_payor_information or has_all_information_required state.");

        _state = PayeeState::has_all_information_required;
        _contractOutPoint = contractOutPoint;
        _payorContractPk = payorContractPk;
        _payorFinalPk = payorFinalPk;
        _funds = funds;
    }

    Commitment Payee::commitment() const {

        if(_state != PayeeState::has_all_information_required)
            throw std::runtime_error("State incompatible request, must be has_all_information_required state.");

        return Commitment(_funds,
                          _payorContractPk,
                          _payeeContractKeys.pk());
    }

    Refund Payee::refund() const {

        if(_state != PayeeState::has_all_information_required)
            throw std::runtime_error("State incompatible request, must be has_all_information_required state.");

        // *** no fee !!!
        return Refund(contractOutPoint(),
                      commitment(),
                      Coin::Payment(_funds, _payorFinalPk.toPubKeyHash()),
                      _lockTime);
    }

    Settlement Payee::settlement(int64_t paymentCount) const {

        // Setup new payment outputs
        int64_t paid = _price * paymentCount;

        Q_ASSERT(paid <= _funds);

        /*
        return Settlement(contractOutPoint(),
                          commitment(),
                          Coin::Payment(_funds - paid, _payorFinalPk.toPubKeyHash()),
                          Coin::Payment(paid - (BITCOIN_DUST_LIMIT + PAYCHAN_SETTLEMENT_FEE), _payeePaymentKeys.pk().toPubKeyHash()));
                          */

        return Settlement::dustLimitAndFeeAwareSettlement(contractOutPoint(),
                                                          commitment(),
                                                          _payorFinalPk.toPubKeyHash(),
                                                          _payeePaymentKeys.pk().toPubKeyHash(),
                                                          _funds,
                                                          paid,
                                                          PAYCHAN_SETTLEMENT_FEE);
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

        // Create settlement
        Settlement s = settlement(_numberOfPaymentsMade);

        // Create payee signature
        Coin::TransactionSignature payeeTransactionSignature = s.transactionSignature(_payeeContractKeys.sk());

        // Create payor signature
        Coin::TransactionSignature payorTransactionSignature = Coin::TransactionSignature(_lastValidPayorPaymentSignature, Coin::SigHashType::standard());

        // Return signed transaction
        return s.signedTransaction(payorTransactionSignature, payeeTransactionSignature);
    }

    PayeeState Payee::state() const {
        return _state;
    }

    void Payee::setState(PayeeState state) {
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

}
}
