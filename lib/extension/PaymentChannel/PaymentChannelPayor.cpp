#include "PaymentChannelPayor.hpp"
#include "RefundTansaction.hpp"
#include "PaymentTransaction.hpp"

PaymentChannelPayor::Slot::Slot() {
}

PaymentChannelPayor::Slot::Slot(const Slot& slot) {
    _state = slot.state();
    _numberOfPaymentsMade = slot.numberOfPaymentsMade();
    _funds = slot.funds();
    _priceIncrement = slot.priceIncrement();
    _index = slot.index();
    _payorKeyPair = slot.payorKeyPair();
    _payeeFinalPk = slot.payeeFinalPk();
    _refund = slot.refund();
}

PaymentChannelPayor::Slot & PaymentChannelPayor::Slot::operator=(const Slot& rhs) {
    _state = rhs.state();
    _numberOfPaymentsMade = rhs.numberOfPaymentsMade();
    _funds = rhs.funds();
    _priceIncrement = rhs.priceIncrement();
    _index = rhs.index();
    _payorKeyPair = rhs.payorKeyPair();
    _payeeFinalPk = rhs.payeeFinalPk();
    _refund = rhs.refund();
    return *this;
}

PaymentChannelPayor::Slot::Slot(const State & state,
                                quint64 numberOfPaymentsMade,
                                quint64 funds,
                                quint64 priceIncrement,
                                quint32 index,
                                const KeyPair& payorKeyPair,
                                const PublicKey & payeeContractPk,
                                const PublicKey & payeeFinalPk,
                                const Signature & refund)
                                : _state(state)
                                , _numberOfPaymentsMade(numberOfPaymentsMade)
                                , _funds(funds)
                                , _priceIncrement(priceIncrement)
                                , _index(index)
                                , _payorKeyPair(payorKeyPair)
                                , _payeeContractPk(payeeContractPk)
                                , _payeeFinalPk(payeeFinalPk)
                                , _refund(refund) {
}

PaymentChannelPayor::Slot::Slot(quint64 funds,
                                 quint64 priceIncrement,
                                 quint32 index,
                                 const KeyPair& payorKeyPair,
                                 const PublicKey & payeeContractPk,
                                 const PublicKey & payeeFinalPk)
                                : _state(State::unassigned_payee)
                                , _numberOfPaymentsMade(0)
                                , _funds(funds)
                                , _priceIncrement(priceIncrement)
                                , _index(index)
                                , _payorKeyPair(payorKeyPair)
                                , _payeeContractPk(payeeContractPk)
                                , _payeeFinalPk(payeeFinalPk)
                                , _refund(refund) {

}

RefundTransaction PaymentChannelPayor::Slot::refundTransaction() const {

    return RefundTransaction();
}

PaymentTransaction PaymentChannelPayor::Slot::paymentTransaction() const {

}

bool PaymentChannelPayor::Slot::isRefundValid(Signature payeeSignature) {

    // Create refund transaction
    // Tx refund = refundTransaction();

    // Create payor refund signature
    // Signature sig1 = _payorKeyPair.sk().sign(refund, sighash... );

    // Check that both can spend multisig output
    // BitCoin::checkMultisignature(contractOutput, sig1, signature);
}

Signature PaymentChannelPayor::Slot::nextPaymentSignature() const {

}

void PaymentChannelPayor::Slot::paymentMade() {

}

bool PaymentChannelPayor::Slot::isRefundDoubleSpent() const {

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

KeyPair PaymentChannelPayor::Slot::payorKeyPair() const {
    return _payorKeyPair;
}

void PaymentChannelPayor::Slot::setPayorKeyPair(const KeyPair &payorKeyPair) {
    _payorKeyPair = payorKeyPair;
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

ContractTransaction PaymentChannelPayor::getContract() const {

}
