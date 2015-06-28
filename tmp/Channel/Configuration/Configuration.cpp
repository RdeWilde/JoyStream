#include "Configuration.hpp"

Channel::Configuration::Payor::Payor() {
}

Channel::Configuration::Payor::Payor(quint64 funds, const KeyPair & contractKeyPair, const KeyPair & finalKeyPair)
    : _funds(funds)
    , _contractKeyPair(contractKeyPair)
    , _finalKeyPair(finalKeyPair) {
}

Channel::Configuration::Payor::Payor(const Channel::Configuration::Payor & o) {
    *this = o;
}

Channel::Configuration::Payor & Channel::Configuration::Payor::operator=(const Payor & o) {

    _funds = o.funds();
    _contractKeyPair = o.contractKeyPair();
    _finalKeyPair = o.finalKeyPair();

    return *this;
}

KeyPair Channel::Configuration::Payor::finalKeyPair() const {
    return _finalKeyPair;
}

void Channel::Configuration::Payor::setFinalKeyPair(const KeyPair & finalKeyPair) {
    _finalKeyPair = finalKeyPair;
}

KeyPair Channel::Configuration::Payor::contractKeyPair() const {
    return _contractKeyPair;
}

void Channel::Configuration::Payor::setContractKeyPair(const KeyPair & contractKeyPair) {
    _contractKeyPair = contractKeyPair;
}

quint64 Channel::Configuration::Payor::funds() const {
    return _funds;
}

void Channel::Configuration::Payor::setFunds(quint64 funds) {
    _funds = funds;
}

Channel::Configuration::Payee::Payee() {
}

Channel::Configuration::Payee::Payee(quint64 price, const PublicKey & contractPk, const PublicKey & finalPk, quint32 refundLockTime)
    : _price(price)
    , _contractPk(contractPk)
    , _finalPk(finalPk)
    , _refundLockTime(refundLockTime) {
}

// Copy constructor
Channel::Configuration::Payee::Payee(const Channel::Configuration::Payee & o) {
    *this = o;
}

// Assignment operator
Channel::Configuration::Payee & Channel::Configuration::Payee::operator=(const Payee & o) {

    _price = o.price();
    _contractPk = o.contractPk();
    _finalPk = o.finalPk();
    _refundLockTime = o.refundLockTime();

    return *this;
}

quint64 Channel::Configuration::Payee::price() const {
    return _price;
}

void Channel::Configuration::Payee::setPrice(quint64 price) {
    _price = price;
}

quint32 Channel::Configuration::Payee::refundLockTime() const {
    return _refundLockTime;
}

void Channel::Configuration::Payee::setRefundLockTime(quint32 refundLockTime) {
    _refundLockTime = refundLockTime;
}

PublicKey Channel::Configuration::Payee::finalPk() const {
    return _finalPk;
}

void Channel::Configuration::Payee::setFinalPk(const PublicKey & finalPk) {
    _finalPk = finalPk;
}

PublicKey Channel::Configuration::Payee::contractPk() const {
    return _contractPk;
}

void Channel::Configuration::Payee::setContractPk(const PublicKey & contractPk) {
    _contractPk = contractPk;
}

Channel::Configuration::Configuration() {

}

Channel::Configuration::Payor Configuration::payorConfiguration() const {
    return _payorConfiguration;
}

void Channel::Configuration::setPayorConfiguration(const Channel::Configuration::Payor & payorConfiguration) {
    _payorConfiguration = payorConfiguration;
}

Channel::Configuration::Payee Channel::Configuration::payeeConfiguration() const {
    return _payeeConfiguration;
}

void Channel::Configuration::setPayeeConfiguration(const Channel::Configuration::Payee & payeeConfiguration) {
    _payeeConfiguration = payeeConfiguration;
}
