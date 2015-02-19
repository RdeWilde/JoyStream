#include "Payor.hpp"

/**
 * Payor::Channel::Configuration::Payor
 */

Payor::Channel::Configuration::Payor::Payor() {

}

Payor::Channel::Configuration::Payor::Payor(quint64 funds, const KeyPair & contractKeyPair, const KeyPair & finalKeyPair)
    : _funds(funds)
    , _contractKeyPair(contractKeyPair)
    , _finalKeyPair(finalKeyPair) {
}

Payor::Channel::Configuration::Payor::Payor(const Payor & o) {
    *this = o;
}

Payor::Channel::Configuration::Payor & Payor::Channel::Configuration::Payor::operator=(const Payor & o) {

    _funds = o.funds();
    _contractKeyPair = o.contractKeyPair();
    _finalKeyPair = o.finalKeyPair();

    return *this;
}

KeyPair Payor::Channel::Configuration::Payor::finalKeyPair() const {
    return _finalKeyPair;
}

void Payor::Channel::Configuration::Payor::setFinalKeyPair(const KeyPair & finalKeyPair) {
    _finalKeyPair = finalKeyPair;
}

KeyPair Payor::Channel::Configuration::Payor::contractKeyPair() const {
    return _contractKeyPair;
}

void Payor::Channel::Configuration::Payor::setContractKeyPair(const KeyPair & contractKeyPair) {
    _contractKeyPair = contractKeyPair;
}

quint64 Payor::Channel::Configuration::Payor::funds() const {
    return _funds;
}

void Payor::Channel::Configuration::Payor::setFunds(quint64 funds) {
    _funds = funds;
}

/**
 * Payor::Channel::Configuration::Payee
 */

Payor::Channel::Configuration::Payee::Payee() {
}

Payor::Channel::Configuration::Payee::Payee(quint64 price, const PublicKey & contractPk, const PublicKey & finalPk, quint32 refundLockTime)
    : _price(price)
    , _contractPk(contractPk)
    , _finalPk(finalPk)
    , _refundLockTime(refundLockTime) {
}

// Copy constructor
Payor::Channel::Configuration::Payee::Payee(const Channel::Configuration::Payee & o) {
    *this = o;
}

// Assignment operator
Payor::Channel::Configuration::Payee & Payor::Channel::Configuration::Payee::operator=(const Channel::Configuration::Payee & o) {

    _price = o.price();
    _contractPk = o.contractPk();
    _finalPk = o.finalPk();
    _refundLockTime = o.refundLockTime();

    return *this;
}

quint64 Payor::Channel::Configuration::Payee::price() const {
    return _price;
}

void Payor::Channel::Configuration::Payee::setPrice(quint64 price) {
    _price = price;
}

quint32 Payor::Channel::Configuration::Payee::refundLockTime() const {
    return _refundLockTime;
}

void Payor::Channel::Configuration::Payee::setRefundLockTime(quint32 refundLockTime) {
    _refundLockTime = refundLockTime;
}

PublicKey Payor::Channel::Configuration::Payee::finalPk() const {
    return _finalPk;
}

void Payor::Channel::Configuration::Payee::setFinalPk(const PublicKey & finalPk) {
    _finalPk = finalPk;
}

PublicKey Payor::Channel::Configuration::Payee::contractPk() const {
    return _contractPk;
}

void Payor::Channel::Configuration::Payee::setContractPk(const PublicKey & contractPk) {
    _contractPk = contractPk;
}

Payor::Channel::Configuration::Configuration() {

}

Payor::Channel::Configuration::Payor Payor::Channel::Configuration::payorConfiguration() const {
    return _payorConfiguration;
}

void Payor::Channel::Configuration::setPayorConfiguration(const Payor::Channel::Configuration::Payor & payorConfiguration) {
    _payorConfiguration = payorConfiguration;
}

Payor::Channel::Configuration::Payee Payor::Channel::Configuration::payeeConfiguration() const {
    return _payeeConfiguration;
}

void Payor::Channel::Configuration::setPayeeConfiguration(const Payor::Channel::Configuration::Payee & payeeConfiguration) {
    _payeeConfiguration = payeeConfiguration;
}

/**
 * Payor::Channel
 */

Payor::Channel::Channel() {
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

void Payor::Channel::computePayorRefundSignature(const Hash &contractHash) const {

    // Check that channel has been assigned
    if(_state != State::assigned)
        throw std::exception("State incompatile request, must be in assigned state.");

    // Make call to compute signature
    OutPoint contractOutputPoint(contractHash, _index);
    P2PKHTxOut refundOutput(_funds, _payorFinalKeyPair.pk());

    // remove PKs later, no reason we need them

    _payorRefundSignature = BitSwaprjs.compute_payor_refund_signature(contractOutputPoint, _payorContractKeyPair.sk(), _payorContractKeyPair.pk(), _payeeContractPk, refundOutput,_refundLockTime);
}

Signature Payor::Channel::paymentSignature(const Hash &contractHash) const {

    // Create pamynent
    //Payment payment = payment(contractHash);

    /**
      QJsonObject raw = payment.rawTransaction();
      return _payorContractKeyPair.sk().sign(raw, sighash);
      */

    return Signature();
}


void Payor::Channel::paymentMade() {
    _numberOfPaymentsMade++;
}

QJsonObject Payor::Channel::json() const {
    return QJsonObject {
                {"funds", _funds},
                {"firstPk", _payorContractKeyPair.pk().toString()},
                {"secondPk", _payeeContractPk.toString()}
            };
}
/*
void Channel::setPayorConfiguration(const Channel::Configuration::Payor & payorConfiguration) {

}

void Channel::setPayeeConfiguration(const Channel::Configuration::Payee & payeeConfiguration) {

}

*/


/**
 * Payor::Configuration
 */

Payor::Configuration::Configuration() {

}

/**
 * Payor
 */

#include "extension/BitCoin/P2SHTxOut.hpp"
#include "extension/BitCoin/BitSwaprjs.hpp"

Payor::Payor() {
}

Payor::Payor(const Payor::Configuration & configuration) {

}

Payor::Payor(const OutPoint& fundingOutput, const KeyPair& fundingOutputKeyPair)
    : _state(State::waiting_for_full_set_of_sellers)
    , _fundingOutput(fundingOutput)
    , _fundingOutputKeyPair(fundingOutputKeyPair)
    , _numberOfSignatures(0) {

    // Check that _fundingOutput
    // *exists
    // *is unspent
    // *has correct output script with correct sighash
    // *is controlled by _fundingOutputKeyPair
    // *has enough value
}

/*
Payor::Payor(const QSet<Channel::PayorConfiguration> & configurations, const OutPoint& fundingOutput, const KeyPair& fundingOutputKeyPair)
    : _state(State::waiting_for_full_set_of_sellers)
    , _fundingOutput(fundingOutput)
    , _fundingOutputKeyPair(fundingOutputKeyPair)
    , _numberOfSignedSlots(0) {

    // Check that _fundingOutput
    // *exists
    // *is unspent
    // *has correct output script with correct sighash
    // *is controlled by _fundingOutputKeyPair
    // *has enough value

    quint32 index = 0;
    for(QSet<Channel::PayorConfiguration>::iterator i = configurations.begin(),
            end(configurations.end()); i != end;i++) {

        // Get configuration
        Channel::PayorConfiguration payorConfiguration = *i;

        // Add slot
        _channels.append(s);

        // Increment counter
        index++;
    }
}
*/

quint32 Payor::addChannel(const Channel::Configuration::Payor & configuration) {
/*
    // Check state
    if(_state != State::waiting_for_full_set_of_sellers)
        throw std::exception("State incompatile request, must be in waiting_for_full_set_of_sellers state.");

    // Get channel size
    quint32 index = _channels.size();

    // Create channel
    Channel c;
    c.index(index);
    c.state(Channel::State::unassigned);
    c.numberOfPaymentsMade(0);
    c.payorContractKeyPair(contractKeyPair);
    c.payorFinalKeyPair(finalKeyPair);
    c.refundFee(refundFee);
    c.funds(funds);

    // Add channel to vector of channels.
    _channels.p(c);

    // Return size of channel
    return index + 1;*/

    return 0;
}

quint32 Payor::assignUnassignedSlot(const Channel::Configuration::Payee & configuration) {
/*
    // Check payor is trying to find sellers
    if(_state != State::waiting_for_full_set_of_sellers)
        throw std::exception("State incompatile request, must be in waiting_for_full_set_of_sellers state.");

    // Find a slot
    quint32 numberUnassigned = 0;
    quint32 slotIndex = -1;
    for(quint32 i = 0;i < _channels.size();i++) {

        // Get reference to slot
        Channel & s = _channels[i];

        if(s.state() == Channel::State::unassigned) {

            // Count number of unassigned slots
            numberUnassigned++;

            // Pick first unassigned slot
            if(numberUnassigned == 0) {

                // And save payee configurations for slot
                s.price(configuration.price());
                s.payeeContractPk(configuration.contractPk());
                s.payeeFinalPk(configuration.finalPk());
                s.paymentFee(configuration.paymentFee());
                s.state(Channel::State::assigned);

                // and slot index
                slotIndex = i;
            }
        }
    }

    // If this was the last unasigned slot, then alter state
    if(numberUnassigned == 1) {
        _state = State::waiting_for_full_set_of_refund_signatures;

        // Compute and set _contractHash
        P2PKHTxOut changeOutput(_changeValue, _changeOutputKeyPair.pk());
        _contractHash = BitSwaprjs.compute_contract_hash(_fundingOutput, _fundingOutputKeyPair.pk(), _channels, changeOutput);

        // Compute all refund signatures
        for(QVector<Channel>::Iterator i = _channels.begin(), end(_channels.end()); i != end;i++)
            i->computePayorRefundSignature(_contractHash);

    }

    return slotIndex;

    */

    return 0;
}

void Payor::unassignSlot(quint32 index) {

    if(_state != State::waiting_for_full_set_of_sellers && _state != State::waiting_for_full_set_of_refund_signatures)
        throw std::exception("State incompatile request, must be in State::waiting_for_full_set_of_sellers or State::waiting_for_full_set_of_refund_signatures state.");

    // Check that index is valid
    if(index >= _channels.size())
        throw std::exception("Invalid index.");

    // Update slot state
    _channels[index].state(Channel::State::unassigned);

    // If some slots had refund signed state, i.e. valid signature, then revert state
    for(QVector<Channel>::iterator i = _channels.begin(),
            end(_channels.end()); i != end;i++) {

        // Get slot
        Channel & s = *i;

        // Reset state back to simply assigned, if a valid signature existed
        if(s.state() == Channel::State::refund_signed)
            s.state(Channel::State::assigned);
    }

    // Set state of payor
    _state = State::waiting_for_full_set_of_sellers;

    // No signature is now valid
    _numberOfSignatures = 0;
}

bool Payor::processRefundSignature(quint32 index, const Signature & signature) {

    // Check that refunds are being collected
    if(_state != State::waiting_for_full_set_of_refund_signatures)
        throw std::exception("State incompatile request, must be in State::waiting_for_full_set_of_refund_signatures state.");

    // Check that index is valid
    if(index >= _channels.size())
        throw std::exception("Invalid index.");

    // Get channel
    Channel & s = _channels[index];

    Q_ASSERT(s.state() == Channel::State::assigned);

    // Check signature
    bool validSignature = BitSwaprjs.check_refund_signatures(OutPoint(_contractHash, index),
                                                             s.payorRefundSignature(),
                                                             signature,
                                                             s.payorContractKeyPair().pk(),
                                                             s.payeeContractPk(),
                                                             P2PKHTxOut(s.funds(), s.payorFinalPk()),
                                                             s.refundLockTime());

    // If it matched, then alter state and save signature
    if(validSignature) {

        s.state(Channel::State::refund_signed);
        s.setPayeeRefundSignature(signature);

        _numberOfSignatures++;

        // Check if they are all signed
        if(_numberOfSignatures == _channels.size()) {
            _state = State::paying;

            // What do we do now?!
            // up to user of channel to act.
        }
    }

    return validSignature;
}

Signature Payor::getPresentPaymentSignature(quint32 index) const {

    // Check that we are paying
    if(_state != State::paying)
        throw std::exception("State incompatile request, must be in State::paying state.");

    // Check that index is valid
    if(index >= _channels.size())
        throw std::exception("Invalid index.");

    // Get channel
    Channel & s = _channels[index];

    Q_ASSERT(s.state() == Channel::State::refund_signed);

    // The amount paid so far
    quint64 amountPaid = _price*_numberOfPaymentsMade;

    return BitSwaprjs.compute_payor_payment_signature(OutPoint(_contractHash, index),
                                                       P2PKHTxOut(s.funds() - amountPaid, s.payorContractKeyPair.pk()),
                                                       P2PKHTxOut(amountPaid - _paymentFee, s.payorContractKeyPair.pk()),
                                                       s.payorContractKeyPair().sk());
}

bool Payor::claimRefund(quint32 index) const {

    // Try to spend refund
}

bool Payor::spent(quint32 index) const {

    // Check if cannel payment has been spent
}

/**
Contract Payor::contract() const {

    // Check that a full set of sellers has been established
    if(_state != State::waiting_for_full_set_of_refund_signatures)
        throw std::exception("State incompatile request, must be in State::waiting_for_full_set_of_refund_signatures state.");

    // Build contract
    Contract contract(_fundingOutput, _slots.size(), P2PKHTxOut(_changeValue, _changeOutputKeyPair.pk()));

    // Set outputs
    for(QVector<Slot>::iterator i = _slots.begin(), end(_slots.end()); i != end;i++)
        contract.setOutput(P2SHTxOut(*i.funds(), *i.payorContractKeyPair(), *i.payeeContractPk()));

    return contract;
}
*/

OutPoint Payor::fundingOutput() const {
    return _fundingOutput;
}

void Payor::setFundingOutput(const OutPoint &fundingOutput) {
    _fundingOutput = fundingOutput;
}

quint32 Payor::numberOfSignatures() const {
    return _numberOfSignatures;
}

void Payor::setNumberOfSignatures(const quint32 &numberOfSignedSlots) {
    _numberOfSignatures = numberOfSignedSlots;
}

