#include "Channel.hpp"


#include "extension/BitCoin/BitSwaprjs.hpp"

Channel::PayorConfiguration::PayorConfiguration(quint64 funds, const KeyPair &contractKeyPair, const KeyPair &finalKeyPair, quint64 refundFee)
    : _funds(funds)
    , _contractKeyPair(contractKeyPair)
    , _finalKeyPair(finalKeyPair)
    , _refundFee(refundFee)
    , _refundLockTime(refundLockTime){
}

quint64 Channel::PayorConfiguration::funds() const {
    return _funds;
}

void Channel::PayorConfiguration::setFunds(const quint64 &funds) {
    _funds = funds;
}

KeyPair Channel::PayorConfiguration::contractKeyPair() const {
    return _contractKeyPair;
}

void Channel::PayorConfiguration::setContractKeyPair(const KeyPair &contractKeyPair) {
    _contractKeyPair = contractKeyPair;
}

KeyPair Channel::PayorConfiguration::finalKeyPair() const {
    return _finalKeyPair;
}

void Channel::PayorConfiguration::setFinalKeyPair(const KeyPair &finalKeyPair) {
    _finalKeyPair = finalKeyPair;
}

quint64 Channel::PayorConfiguration::refundFee() const {
    return _refundFee;
}

void Channel::PayorConfiguration::setRefundFee(const quint64 &refundFee) {
    _refundFee = refundFee;
}

quint32 Channel::PayorConfiguration::getRefundLockTime() const {
    return refundLockTime;
}

void Channel::PayorConfiguration::setRefundLockTime(const quint32 &value) {
    refundLockTime = value;
}

Channel::PayeeConfiguration::PayeeConfiguration(quint64 price, const PublicKey &contractPk, const PublicKey &finalPk, quint64 paymentFee)
    : _price(price)
    , _contractPk(contractPk)
    , _finalPk(finalPk)
    , _paymentFee(paymentFee) {

}

quint64 Channel::PayeeConfiguration::paymentFee() const {
    return _paymentFee;
}

void Channel::PayeeConfiguration::setPaymentFee(const quint64 &paymentFee) {
    _paymentFee = paymentFee;
}

PublicKey Channel::PayeeConfiguration::finalPk() const {
    return _finalPk;
}

void Channel::PayeeConfiguration::setFinalPk(const PublicKey &finalPk) {
    _finalPk = finalPk;
}

PublicKey Channel::PayeeConfiguration::contractPk() const {
    return _contractPk;
}

void Channel::PayeeConfiguration::setContractPk(const PublicKey &contractPk) {
    _contractPk = contractPk;
}

quint64 Channel::PayeeConfiguration::price() const {
    return _price;
}

void Channel::PayeeConfiguration::setPrice(const quint64 &price) {
    _price = price;
}

Channel::Channel() {
}

Channel::Channel(const Channel& slot) {
    _index = slot.index();
    _state = slot.state();
    _price = slot.price();
    _numberOfPaymentsMade = slot.numberOfPaymentsMade();
    _funds = slot.funds();
    _payorContractKeyPair = slot.payorContractKeyPair();
    _payeeContractPk = slot.payeeContractPk();
    _payeeFinalPk = slot.payeeFinalPk();
    _payeeRefundSignature = slot.refund();
    _refundFee = slot.refundFee();
    _paymentFee = slot.paymentFee();
    _refundLockTime = slot.refundLockTime();
}

Channel & Channel::operator=(const Channel& rhs) {
    _index = rhs.index();
    _state = rhs.state();
    _price = rhs.price();
    _numberOfPaymentsMade = rhs.numberOfPaymentsMade();
    _funds = rhs.funds();
    _payorContractKeyPair = rhs.payorContractKeyPair();
    _payeeContractPk = rhs.payeeContractPk();
    _payeeFinalPk = rhs.payeeFinalPk();
    _payeeRefundSignature = rhs.refund();
    _refundFee = rhs.refundFee();
    _paymentFee = rhs.paymentFee();
    _refundLockTime = rhs.refundLockTime();
    return *this;
}

Channel::Channel(quint32 index,
                                const State &state,
                                quint64 priceIncrement,
                                quint64 numberOfPaymentsMade,
                                quint64 funds,
                                const KeyPair &payorContractKeyPair,
                                const PublicKey &payeeContractPk,
                                const PublicKey &payeeFinalPk,
                                const Signature &refund,
                                quint64 refundFee,
                                quint64 paymentFee)
                                : _index(index)
                                , _state(state)
                                , _price(priceIncrement)
                                , _numberOfPaymentsMade(numberOfPaymentsMade)
                                , _funds(funds)
                                , _payorContractKeyPair(payorContractKeyPair)
                                , _payeeContractPk(payeeContractPk)
                                , _payeeFinalPk(payeeFinalPk)
                                , _payeeRefundSignature(refund)
                                , _refundFee(refundFee)
                                , _paymentFee(paymentFee) {
}

/**
Refund Channel::refund(const Hash &contractHash) const {
    return Refund(OutputPoint(contractHash, _index),
                  P2PKHTxOut(_funds - _refundFee, _payorContractKeyPair.pk(), _payeeContractPk),
                  _refundLockTime);
}

Payment Channel::payment(const Hash &contractHash) const {

    // The amount paid so far
    quint64 amountPaid = _price*_numberOfPaymentsMade;

    return Payment(OutputPoint(contractHash, _index),
                   P2PKHTxOut(_funds - amountPaid, _payorContractKeyPair.pk()),
                   P2PKHTxOut(amountPaid - _paymentFee, _payorContractKeyPair.pk()));
}
*/

void Channel::computePayorRefundSignature(const Hash &contractHash) const {

    // Check that channel has been assigned
    if(_state != State::assigned)
        throw std::exception("State incompatile request, must be in assigned state.");

    // Make call to compute signature
    OutputPoint contractOutputPoint(contractHash, _index);
    P2PKHTxOut refundOutput(_funds, _payorFinalKeyPair.pk());

    // remove PKs later, no reason we need them

    _payorRefundSignature = BitSwaprjs.compute_payor_refund_signature(contractOutputPoint, _payorContractKeyPair.sk(), _payorContractKeyPair.pk(), _payeeContractPk, refundOutput,_refundLockTime);
}

Signature Channel::paymentSignature(const Hash &contractHash) const {

    // Create pamynent
    Payment payment = payment(contractHash);

    /**
      QJsonObject raw = payment.rawTransaction();
      return _payorContractKeyPair.sk().sign(raw, sighash);
      */

    return Signature();
}

QJsonObject Channel::json() const {
    return QJsonObject {
                          {"funds", _funds},
                          {"firstPk", _payorContractKeyPair.pk().toString()},
                          {"secondPk", _payeeContractPk.toString()}
                       };
}

void Channel::paymentMade() {
    _numberOfPaymentsMade++;
}

Channel::State Channel::state() const {
    return _state;
}

void Channel::setState(const State &state) {
    _state = state;
}

quint64 Channel::numberOfPaymentsMade() const {
    return _numberOfPaymentsMade;
}

void Channel::setNumberOfPaymentsMade(const quint64 &numberOfPaymentsMade) {
    _numberOfPaymentsMade = numberOfPaymentsMade;
}

Signature Channel::refund() const {
    return _payeeRefundSignature;
}

void Channel::setRefund(const Signature &refund) {
    _payeeRefundSignature = refund;
}

quint64 Channel::paymentFee() const {
    return _paymentFee;
}

void Channel::setPaymentFee(const quint64 &paymentFee) {
    _paymentFee = paymentFee;
}

PublicKey Channel::payeeFinalPk() const {
    return _payeeFinalPk;
}

void Channel::setPayeeFinalPk(const PublicKey &payeeFinalPk) {
    _payeeFinalPk = payeeFinalPk;
}


quint64 Channel::refundFee() const {
    return _refundFee;
}

void Channel::setRefundFee(const quint64 &refundFee) {
    _refundFee = refundFee;
}

quint32 Channel::index() const {
    return _index;
}

void Channel::setIndex(const quint32 &index) {
    _index = index;
}

PublicKey Channel::payeeFinalPk() const {
    return _payeeFinalPk;
}

void Channel::setPayeeFinalPk(const PublicKey &payeeFinalPk) {
    _payeeFinalPk = payeeFinalPk;
}

quint32 Channel::refundLockTime() const {
    return _refundLockTime;
}

void Channel::setRefundLockTime(const quint32 &refundLockTime) {
    _refundLockTime = refundLockTime;
}

Signature Channel::payeeRefundSignature() const
{
    return _payeeRefundSignature;
}

void Channel::setPayeeRefundSignature(const Signature &payeeRefundSignature)
{
    _payeeRefundSignature = payeeRefundSignature;
}

Signature Channel::payorRefundSignature() const
{
    return _payorRefundSignature;
}

void Channel::setPayorRefundSignature(const Signature &payorRefundSignature)
{
    _payorRefundSignature = payorRefundSignature;
}

PublicKey Channel::payeeContractPk() const {
    return _payeeContractPk;
}

void Channel::setPayeeContractPk(const PublicKey &payeeContractPk) {
    _payeeContractPk = payeeContractPk;
}

KeyPair Channel::payorContractKeyPair() const {
    return _payorContractKeyPair;
}

void Channel::setPayorContractKeyPair(const KeyPair &payorKeyPair) {
    _payorContractKeyPair = payorKeyPair;
}

quint32 Channel::index() const {
    return _index;
}

void Channel::setIndex(const quint32 &index) {
    _index = index;
}

quint64 Channel::price() const {
    return _price;
}

void Channel::setPrice(const quint64 &priceIncrement) {
    _price = priceIncrement;
}

quint64 Channel::funds() const {
    return _funds;
}

void Channel::setFunds(const quint64 &funds) {
    _funds = funds;
}
