#include "Payee.hpp"

#include "extension/BitCoin/P2PKHTxOut.hpp"
#include "extension/BitCoin/BitSwaprjs.hpp"

/**
 * Payee::Status
 */

Payee::Status::Status() {
}

Payee::Status::Status(State state, quint64 numberOfPaymentsMade, quint32 lockTime, quint64 price, quint64 funds)
    : _state(state)
    , _numberOfPaymentsMade(numberOfPaymentsMade)
    , _lockTime(lockTime)
    , _price(price)
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

Payee::Configuration::Configuration(quint32 lockTime, quint64 price, quint32 maximumNumberOfSellers, const KeyPair & payeeContractKeys, const KeyPair & payeePaymentKeys)
    : _lockTime(lockTime)
    , _price(price)
    , _maximumNumberOfSellers(maximumNumberOfSellers)
    , _payeeContractKeys(payeeContractKeys)
    , _payeePaymentKeys(payeePaymentKeys) {
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

PublicKey Payee::Configuration::payeeContractPk() const {
    return _payeeContractKeys;
}

void Payee::Configuration::setPayeeContractPk(const PublicKey & payeeContractKeys) {
    _payeeContractKeys = payeeContractKeys;
}

PublicKey Payee::Configuration::payeePaymentPk() const {
    return _payeePaymentKeys;
}

void Payee::Configuration::setPayeePaymentPk(const PublicKey & payeePaymentKeys) {
    _payeePaymentKeys = payeePaymentKeys;
}

/**
 * Payee
 */

Payee::Payee()
    : _state(State::waiting_for_payee_information)
    , _numberOfPaymentsMade(0) {
}

Payee::Payee(const Payee::Configuration & c, const PrivateKey & payeeContractSk, const PrivateKey & payeePaymentSk)
    : _state(State::waiting_for_payor_information)
    , _numberOfPaymentsMade(0)
    , _lockTime(c.lockTime())
    , _price(c.price())
    , _maximumNumberOfSellers(c.maximumNumberOfSellers())
    , _payeeContractKeys(c.payeeContractPk(), payeeContractSk)
    , _payeePaymentKeys(c.payeePaymentPk(), payeePaymentSk) {
}

void Payee::registerPayeeInformation(quint32 lockTime, quint32 price, quint32 maximumNumberOfSellers, const KeyPair & payeeContractKeys, const KeyPair & payeePaymentKeys) {

    // Check state
    if(_state != State::waiting_for_payee_information)
        throw std::exception("State incompatible request, must be in waiting_for_payee_information state.");

    _state = State::waiting_for_payor_information;
    _lockTime = lockTime;
    _price = price;
    _maximumNumberOfSellers = maximumNumberOfSellers;
    _payeeContractKeys = payeeContractKeys;
    _payeePaymentKeys = payeePaymentKeys;
}

void Payee::registerPayorInformation(const OutPoint & contractOutPoint, const PublicKey & payorContractPk, const PublicKey & payorFinalPk, quint64 funds) {

    // Check state
    if(_state != State::waiting_for_payor_information)
        throw std::exception("State incompatible request, must be in waiting_for_payor_information state.");

    _state = State::has_all_information_required;
    _contractOutPoint = contractOutPoint;
    _payorContractPk = payorContractPk;
    _payorFinalPk = payorFinalPk;
    _funds = funds;
}

Signature Payee::generateRefundSignature() const {

    // Check state
    if(_state != State::has_all_information_required)
        throw std::exception("State incompatile request, must be in has_all_information_required state.");

    // Compute refund signature
    Signature sig = BitSwaprjs::compute_refund_signature(_contractOutPoint,
                                                         _payeeContractKeys.sk(),
                                                         _payorContractPk,
                                                         _payeeContractKeys.pk(),
                                                         P2PKHTxOut(_funds, _payorFinalPk),
                                                         _lockTime);

    return sig;
}

bool Payee::registerPayment(const Signature & paymentSignature) {

    // Check state
    if(_state != State::has_all_information_required)
        throw std::exception("State incompatile request, must be in has_all_information_required state.");

    // Check signature
    bool check = checkNextPaymentSignature(paymentSignature);

    // Increase payment count if signature was valid
    if(check) {
        _numberOfPaymentsMade++;
        _lastValidPayorPaymentSignature = paymentSignature;
    }

    return check;
}

bool Payee::checkNextPaymentSignature(const Signature & payorPaymentSignature) const {

    // Check state
    if(_state != State::has_all_information_required)
        throw std::exception("State incompatile request, must be in has_all_information_required state.");

    // Setup new payment outputs
    quint64 paid = (_numberOfPaymentsMade + 1) * _price;
    P2PKHTxOut refundOutput(_funds - paid, _payorFinalPk);
    P2PKHTxOut paymentOutput(_funds + paid, _payeePaymentKeys.pk());

    // Compute payee signature
    Signature payeePaymentSignature = BitSwaprjs::compute_payment_signature(_contractOutPoint,
                                                                             _payeeContractKeys.sk(),
                                                                             _payorContractPk,
                                                                             _payeeContractKeys.pk(),
                                                                             refundOutput,
                                                                             paymentOutput);
    // Check signature
    return BitSwaprjs::check_payment_signatures(_contractOutPoint,
                                                 payorPaymentSignature,
                                                 payeePaymentSignature,
                                                 _payorContractPk,
                                                 _payeeContractKeys.pk(),
                                                 refundOutput,
                                                 paymentOutput);
}

/**
float Payee::outputPointVisible() const {

    // random bs
    return 0.5;
}

bool Payee::isContractValid() const {

    // random bs
    return true;
}
*/
