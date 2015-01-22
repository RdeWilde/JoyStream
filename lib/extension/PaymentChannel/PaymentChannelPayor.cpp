#include "PaymentChannelPayor.hpp"

PaymentChannelPayor::Slot::Slot() {

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

PaymentChannelPayor::PaymentChannelPayor(quint32 numberOfPayees)
    : _numberOfPayees(numberOfPayees) {
}

