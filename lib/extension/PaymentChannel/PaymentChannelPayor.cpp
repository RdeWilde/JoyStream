#include "PaymentChannelPayor.hpp"
#include "Refund.hpp"
#include "Payment.hpp"

PaymentChannelPayor::Slot::Slot() {
}

PaymentChannelPayor::Slot::Slot(const Slot& slot) {
    _index = slot.index();
    _state = slot.state();
    _priceIncrement = slot.priceIncrement();
    _numberOfPaymentsMade = slot.numberOfPaymentsMade();
    _funds = slot.funds();
    _payorContractKeyPair = slot.payorContractKeyPair();
    _payorFinalPk = slot.payorFinalPk();
    _payeeContractPk = slot.payeeContractPk();
    _payeeFinalPk = slot.payeeFinalPk();
    _refund = slot.refund();
    _refundFee = slot.refundFee();
    _paymentFee = slot.paymentFee();
}

PaymentChannelPayor::Slot & PaymentChannelPayor::Slot::operator=(const Slot& rhs) {
    _index = rhs.index();
    _state = rhs.state();
    _priceIncrement = rhs.priceIncrement();
    _numberOfPaymentsMade = rhs.numberOfPaymentsMade();
    _funds = rhs.funds();
    _payorContractKeyPair = rhs.payorContractKeyPair();
    _payorFinalPk = rhs.payorFinalPk();
    _payeeContractPk = rhs.payeeContractPk();
    _payeeFinalPk = rhs.payeeFinalPk();
    _refund = rhs.refund();
    _refundFee = rhs.refundFee();
    _paymentFee = rhs.paymentFee();
    return *this;
}

PaymentChannelPayor::Slot::Slot(quint32 index,
                                const State &state,
                                quint64 priceIncrement,
                                quint64 numberOfPaymentsMade,
                                quint64 funds,
                                const KeyPair &payorContractKeyPair,
                                const PublicKey &payorFinalPk,
                                const PublicKey &payeeContractPk,
                                const PublicKey &payeeFinalPk,
                                const Signature &refund,
                                quint64 refundFee,
                                quint64 paymentFee)
                                : _index(index)
                                , _state(state)
                                , _priceIncrement(priceIncrement)
                                , _numberOfPaymentsMade(numberOfPaymentsMade)
                                , _funds(funds)
                                , _payorContractKeyPair(payorContractKeyPair)
                                , _payorFinalPk(payorFinalPk)
                                , _payeeContractPk(payeeContractPk)
                                , _payeeFinalPk(payeeFinalPk)
                                , _refund(refund)
                                , _refundFee(refundFee)
                                , _paymentFee(paymentFee) {
}

Refund PaymentChannelPayor::Slot::refund(const Hash &contractHash, quint32 lockTime) const {
    return Refund(OutputPoint(contractHash, _index),
                  P2PKHTxOut(_funds - _refundFee, _payorContractKeyPair.pk(), _payeeContractPk),
                  lockTime);
}

Payment PaymentChannelPayor::Slot::payment(const Hash &contractHash) const {

    // The amount paid so far
    quint64 amountPaid = _priceIncrement*_numberOfPaymentsMade;

    return Payment(OutputPoint(contractHash, _index),
                   P2PKHTxOut(_funds - amountPaid, _payorContractKeyPair.pk()),
                   P2PKHTxOut(amountPaid - _paymentFee, _payorContractKeyPair.pk()));
}

Signature PaymentChannelPayor::Slot::refundSignature(const Hash &contractHash, quint32 lockTime) const {

    // Create refund
    Refund refund = refund(contractHash, lockTime);

    /**
      QJsonObject raw = refund.rawTransaction();
      return _payorContractKeyPair.sk().sign(raw, sighash);
      */

    return Signature();
}

Signature PaymentChannelPayor::Slot::paymentSignature(const Hash &contractHash) const {

    // Create pamynent
    Payment payment = payment(contractHash);

    /**
      QJsonObject raw = payment.rawTransaction();
      return _payorContractKeyPair.sk().sign(raw, sighash);
      */

    return Signature();

}

void PaymentChannelPayor::Slot::paymentMade() {
    _numberOfPaymentsMade++;
}

PaymentChannelPayor::Slot::State PaymentChannelPayor::Slot::state() const {
    return _state;
}

void PaymentChannelPayor::Slot::setState(const State &state) {
    _state = state;
}

quint64 PaymentChannelPayor::Slot::numberOfPaymentsMade() const {
    return _numberOfPaymentsMade;
}

void PaymentChannelPayor::Slot::setNumberOfPaymentsMade(const quint64 &numberOfPaymentsMade) {
    _numberOfPaymentsMade = numberOfPaymentsMade;
}

Signature PaymentChannelPayor::Slot::refund() const {
    return _refund;
}

void PaymentChannelPayor::Slot::setRefund(const Signature &refund) {
    _refund = refund;
}

quint64 Slot::paymentFee() const {
    return _paymentFee;
}

void PaymentChannelPayor::Slot::setPaymentFee(const quint64 &paymentFee) {
    _paymentFee = paymentFee;
}

quint64 PaymentChannelPayor::Slot::refundFee() const {
    return _refundFee;
}

void PaymentChannelPayor::Slot::setRefundFee(const quint64 &refundFee) {
    _refundFee = refundFee;
}

PublicKey PaymentChannelPayor::Slot::payorFinalPk() const {
    return _payorFinalPk;
}

void PaymentChannelPayor::Slot::setPayorFinalPk(const PublicKey &payorFinalPk) {
    _payorFinalPk = payorFinalPk;
}

quint32 PaymentChannelPayor::Slot::index() const {
    return _index;
}

void PaymentChannelPayor::Slot::setIndex(const quint32 &index) {
    _index = index;
}

PublicKey PaymentChannelPayor::Slot::payeeFinalPk() const {
    return _payeeFinalPk;
}

void PaymentChannelPayor::Slot::setPayeeFinalPk(const PublicKey &payeeFinalPk) {
    _payeeFinalPk = payeeFinalPk;
}

PublicKey PaymentChannelPayor::Slot::payeeContractPk() const {
    return _payeeContractPk;
}

void PaymentChannelPayor::Slot::setPayeeContractPk(const PublicKey &payeeContractPk) {
    _payeeContractPk = payeeContractPk;
}

KeyPair PaymentChannelPayor::Slot::payorContractKeyPair() const {
    return _payorContractKeyPair;
}

void PaymentChannelPayor::Slot::setPayorContractKeyPair(const KeyPair &payorKeyPair) {
    _payorContractKeyPair = payorKeyPair;
}

quint32 PaymentChannelPayor::Slot::index() const {
    return _index;
}

void PaymentChannelPayor::Slot::setIndex(const quint32 &index) {
    _index = index;
}

quint64 PaymentChannelPayor::Slot::priceIncrement() const {
    return _priceIncrement;
}

void PaymentChannelPayor::Slot::setPriceIncrement(const quint64 &priceIncrement) {
    _priceIncrement = priceIncrement;
}

quint64 PaymentChannelPayor::Slot::funds() const {
    return _funds;
}

void PaymentChannelPayor::Slot::setFunds(const quint64 &funds) {
    _funds = funds;
}

PaymentChannelPayor::PaymentChannelPayor() {

}

PaymentChannelPayor::PaymentChannelPayor(quint32 numberOfPayees, const OutputPoint& fundingOutput, const KeyPair& fundingOutputKeyPair)
    : _state(State::constructing_channel)
    , _fundingOutput(fundingOutput)
    , _fundingOutputKeyPair(fundingOutputKeyPair){

    // Check that _fundingOutput
    // *exists
    // *is unspent
    // *has correct output script with correct sighash
    // *is controlled by _fundingOutputKeyPair
    // *has enough value

    // Construct slots
    for(int i = 0;i < numberOfPayees;i++) {

        _slots.append(Slot());

        KeyPair payor =
        _slots.append(Slot(funds_i, p_i,i, K, payor))

    }
}

Contract PaymentChannelPayor::getContract() const {

}

Refund PaymentChannelPayor::refundTransaction(quint32 index) const {
    return _slots[index].refundTransaction(_refundLockTime);
}

bool PaymentChannelPayor::spent(quint32 index) const {

}
