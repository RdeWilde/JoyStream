/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <paymentchannel/Payee.hpp>
#include <paymentchannel/Commitment.hpp>
#include <paymentchannel/Refund.hpp>
#include <paymentchannel/Settlement.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/TransactionSignature.hpp>
#include <common/SigHashType.hpp>
#include <common/Payment.hpp>

#include <CoinCore/CoinNodeData.h>

namespace joystream {
namespace paymentchannel {

    Payee::Payee()
        : _numberOfPaymentsMade(0)
        , _lockTime(0)
        , _price(0)
        , _funds(0)
        , _settlementFee(0){
    }

    Payee::Payee(quint64 numberOfPaymentsMade,
                 quint32 lockTime,
                 quint64 price,
                 quint64 funds,
                 quint64 settlementFee,
                 const Coin::typesafeOutPoint & contractOutPoint,
                 const Coin::KeyPair & payeeContractKeys,
                 const Coin::RedeemScriptHash & payeeFinalKeyHash,
                 const Coin::PublicKey & payorContractPk,
                 const Coin::RedeemScriptHash & payorFinalKeyHash,
                 const Coin::Signature & lastValidPayorPaymentSignature)
        : _numberOfPaymentsMade(numberOfPaymentsMade)
        , _lockTime(lockTime)
        , _price(price)
        , _funds(funds)
        , _settlementFee(settlementFee)
        , _contractOutPoint(contractOutPoint)
        , _payeeContractKeys(payeeContractKeys)
        , _payeeFinalScriptHash(payeeFinalKeyHash)
        , _payorContractPk(payorContractPk)
        , _payorFinalScriptHash(payorFinalKeyHash)
        , _lastValidPayorPaymentSignature(lastValidPayorPaymentSignature) {
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

    Commitment Payee::commitment() const {

        return Commitment(_funds,
                          _payorContractPk,
                          _payeeContractKeys.pk(),
                          _lockTime);
    }

    Settlement Payee::settlement(int numberOfPayments) const {

        quint64 amountPaid = numberOfPayments * _price;

        return Settlement::dustLimitAndFeeAwareSettlement(contractOutPoint(),
                                                          commitment(),
                                                          _payorFinalScriptHash,
                                                          _payeeFinalScriptHash,
                                                          _funds,
                                                          amountPaid,
                                                          _settlementFee);
    }

    bool Payee::isContractValid(const Coin::Transaction &) const {
        return true; // throw std::runtime_error("Not yet implemented");
    }

    quint64 Payee::amountPaid() const {
        return _price*_numberOfPaymentsMade;
    }

    quint64 Payee::numberOfPaymentsMade() const {
        return _numberOfPaymentsMade;
    }

    void Payee::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
        _numberOfPaymentsMade = numberOfPaymentsMade;
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

    quint64 Payee::funds() const {
        return _funds;
    }

    void Payee::setFunds(quint64 funds) {
        _funds = funds;
    }

    quint64 Payee::settlementFee() const {
        return _settlementFee;
    }

    void Payee::setSettlementFee(quint64 settlementFee) {
        _settlementFee = settlementFee;
    }

    Coin::typesafeOutPoint Payee::contractOutPoint() const {
        return _contractOutPoint;
    }

    void Payee::setContractOutPoint(const Coin::typesafeOutPoint & contractOutPoint) {
        _contractOutPoint = contractOutPoint;
    }

    Coin::KeyPair Payee::payeeContractKeys() const {
        return _payeeContractKeys;
    }

    void Payee::setPayeeContractKeys(const Coin::KeyPair & payeeContractKeys) {
        _payeeContractKeys = payeeContractKeys;
    }

    Coin::RedeemScriptHash Payee::payeeFinalScriptHash() const {
        return _payeeFinalScriptHash;
    }

    void Payee::setPayeeFinalScriptHash(const Coin::RedeemScriptHash & payeeFinalKeyHash) {
        _payeeFinalScriptHash = payeeFinalKeyHash;
    }

    Coin::PublicKey Payee::payorContractPk() const {
        return _payorContractPk;
    }

    void Payee::setPayorContractPk(const Coin::PublicKey & payorContractPk) {
        _payorContractPk = payorContractPk;
    }

    Coin::RedeemScriptHash Payee::payorFinalScriptHash() const {
        return _payorFinalScriptHash;
    }

    void Payee::setPayorFinalScriptHash(const Coin::RedeemScriptHash & payorFinalKeyHash) {
        _payorFinalScriptHash = payorFinalKeyHash;
    }

    Coin::Signature Payee::lastValidPayorPaymentSignature() const {
        return _lastValidPayorPaymentSignature;
    }

    void Payee::setLastValidPayorPaymentSignature(const Coin::Signature & lastValidPayorPaymentSignature) {
        _lastValidPayorPaymentSignature = lastValidPayorPaymentSignature;
    }

}
}
