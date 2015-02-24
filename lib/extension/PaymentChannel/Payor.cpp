#include "Payor.hpp"

/**
 * Payor::Channel::PayorSettings
 */

Payor::Channel::PayorSettings::PayorSettings() {

}

Payor::Channel::PayorSettings::PayorSettings(quint64 funds, const KeyPair & contractKeyPair, const KeyPair & finalKeyPair)
    : _funds(funds)
    , _contractKeyPair(contractKeyPair)
    , _finalKeyPair(finalKeyPair) {
}

Payor::Channel::PayorSettings::PayorSettings(const PayorSettings & o) {
    *this = o;
}

Payor::Channel::PayorSettings & Payor::Channel::PayorSettings::operator=(const PayorSettings & o) {

    _funds = o.funds();
    _contractKeyPair = o.contractKeyPair();
    _finalKeyPair = o.finalKeyPair();

    return *this;
}

KeyPair Payor::Channel::PayorSettings::finalKeyPair() const {
    return _finalKeyPair;
}

void Payor::Channel::PayorSettings::setFinalKeyPair(const KeyPair & finalKeyPair) {
    _finalKeyPair = finalKeyPair;
}

KeyPair Payor::Channel::PayorSettings::contractKeyPair() const {
    return _contractKeyPair;
}

void Payor::Channel::PayorSettings::setContractKeyPair(const KeyPair & contractKeyPair) {
    _contractKeyPair = contractKeyPair;
}

quint64 Payor::Channel::PayorSettings::funds() const {
    return _funds;
}

void Payor::Channel::PayorSettings::setFunds(quint64 funds) {
    _funds = funds;
}

/**
 * Payor::Channel::PayeeSettings
 */

Payor::Channel::PayeeSettings::PayeeSettings() {
}

Payor::Channel::PayeeSettings::PayeeSettings(quint64 price, const PublicKey & contractPk, const PublicKey & finalPk, quint32 refundLockTime)
    : _price(price)
    , _contractPk(contractPk)
    , _finalPk(finalPk)
    , _refundLockTime(refundLockTime) {
}

// Copy constructor
Payor::Channel::PayeeSettings::PayeeSettings(const Channel::PayeeSettings & o) {
    *this = o;
}

// Assignment operator
Payor::Channel::PayeeSettings & Payor::Channel::PayeeSettings::operator=(const Payor::Channel::PayeeSettings & o) {

    _price = o.price();
    _contractPk = o.contractPk();
    _finalPk = o.finalPk();
    _refundLockTime = o.refundLockTime();

    return *this;
}

quint64 Payor::Channel::PayeeSettings::price() const {
    return _price;
}

void Payor::Channel::PayeeSettings::setPrice(quint64 price) {
    _price = price;
}

quint32 Payor::Channel::PayeeSettings::refundLockTime() const {
    return _refundLockTime;
}

void Payor::Channel::PayeeSettings::setRefundLockTime(quint32 refundLockTime) {
    _refundLockTime = refundLockTime;
}

PublicKey Payor::Channel::PayeeSettings::finalPk() const {
    return _finalPk;
}

void Payor::Channel::PayeeSettings::setFinalPk(const PublicKey & finalPk) {
    _finalPk = finalPk;
}

PublicKey Payor::Channel::PayeeSettings::contractPk() const {
    return _contractPk;
}

void Payor::Channel::PayeeSettings::setContractPk(const PublicKey & contractPk) {
    _contractPk = contractPk;
}

/**
 * @brief Payor::Channel::Configuration

PayorSettings::Channel::Configuration::Configuration() {

}

Payor::Channel::PayorSettings PayorSettings::Channel::Configuration::payorConfiguration() const {
    return _payorConfiguration;
}

void PayorSettings::Channel::Configuration::setPayorConfiguration(const PayorSettings & payorConfiguration) {
    _payorConfiguration = payorConfiguration;
}

Payor::Channel::PayeeSettings PayorSettings::Channel::Configuration::payeeConfiguration() const {
    return _payeeConfiguration;
}

void PayorSettings::Channel::Configuration::setPayeeConfiguration(const Payee & payeeConfiguration) {
    _payeeConfiguration = payeeConfiguration;
}
*/

/**
 * Payor::Channel
 */

#include "extension/BitCoin/P2PKHTxOut.hpp"
#include "extension/BitCoin/BitSwaprjs.hpp"

#include <QJsonObject>

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

void Payor::Channel::computePayorRefundSignature(const Hash &contractHash) {

    // Check that channel has been assigned
    if(_state != State::assigned)
        throw std::exception("State incompatile request, must be in assigned state.");

    // Make call to compute signature
    OutPoint contractOutPoint(contractHash, _index);
    P2PKHTxOut refundTxOut(_funds, _payorFinalKeyPair.pk());

    // remove PKs later, no reason we need them

    _payorRefundSignature = BitSwaprjs::compute_payor_refund_signature(contractOutPoint,
                                                                      _payorContractKeyPair.sk(),
                                                                      _payorContractKeyPair.pk(),
                                                                      _payeeContractPk,
                                                                      refundTxOut,
                                                                      _refundLockTime);
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
                {"funds", static_cast<qint64>(_funds)},
                {"firstPk", _payorContractKeyPair.pk().toString()},
                {"secondPk", _payeeContractPk.toString()}
            };
}

quint32 Payor::Channel::index() const {
    return _index;
}

Payor::Channel::State Payor::Channel::state() const {
    return _state;
}

void Payor::Channel::setState(const State & state) {
    _state = state;
}

quint32 Payor::Channel::refundLockTime() const {
    return _refundLockTime;
}

void Payor::Channel::setRefundLockTime(quint32 refundLockTime) {
    _refundLockTime = refundLockTime;
}

quint64 Payor::Channel::paymentFee() const {
    return _paymentFee;
}

void Payor::Channel::setPaymentFee(quint64 paymentFee) {
    _paymentFee = paymentFee;
}

quint64 Payor::Channel::refundFee() const {
    return _refundFee;
}

void Payor::Channel::setRefundFee(quint64 refundFee) {
    _refundFee = refundFee;
}

Signature Payor::Channel::payeeRefundSignature() const {
    return _payeeRefundSignature;
}

void Payor::Channel::setPayeeRefundSignature(const Signature & payeeRefundSignature) {
    _payeeRefundSignature = payeeRefundSignature;
}

Signature Payor::Channel::payorRefundSignature() const {
    return _payorRefundSignature;
}

void Payor::Channel::setPayorRefundSignature(const Signature & payorRefundSignature) {
    _payorRefundSignature = payorRefundSignature;
}

PublicKey Payor::Channel::payeeFinalPk() const {
    return _payeeFinalPk;
}

void Payor::Channel::setPayeeFinalPk(const PublicKey & payeeFinalPk) {
    _payeeFinalPk = payeeFinalPk;
}

PublicKey Payor::Channel::payeeContractPk() const {
    return _payeeContractPk;
}

void Payor::Channel::setPayeeContractPk(const PublicKey & payeeContractPk) {
    _payeeContractPk = payeeContractPk;
}

KeyPair Payor::Channel::payorFinalKeyPair() const {
    return _payorFinalKeyPair;
}

void Payor::Channel::setPayorFinalKeyPair(const KeyPair & payorFinalKeyPair) {
    _payorFinalKeyPair = payorFinalKeyPair;
}

KeyPair Payor::Channel::payorContractKeyPair() const {
    return _payorContractKeyPair;
}

void Payor::Channel::setPayorContractKeyPair(const KeyPair & payorContractKeyPair) {
    _payorContractKeyPair = payorContractKeyPair;
}

quint64 Payor::Channel::funds() const {
    return _funds;
}

void Payor::Channel::setFunds(quint64 funds) {
    _funds = funds;
}

quint64 Payor::Channel::numberOfPaymentsMade() const {
    return _numberOfPaymentsMade;
}

void Payor::Channel::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
    _numberOfPaymentsMade = numberOfPaymentsMade;
}

quint64 Payor::Channel::price() const {
    return _price;
}

void Payor::Channel::setPrice(quint64 price) {
    _price = price;
}


void Payor::Channel::setIndex(quint32 index) {
    _index = index;
}

/**
 * Payor::Status
 */

Payor::Status::Status(const QVector<Channel::Status> & channels,
                      State state,
                      quint32 numberOfSignatures)
    : _channels(channels)
    , _state(state)
    , _numberOfSignatures(numberOfSignatures) {
}

QVector<Channel::Status> Payor::Status::channels() const {
    return _channels;
}

void Payor::Status::setChannels(const QVector<Channel::Status> & channels) {
    _channels = channels;
}

Payor::State Payor::Status::state() const {
    return _state;
}

void Payor::Status::setState(State state) {
    _state = state;
}

quint32 Payor::Status::numberOfSignatures() const {
    return _numberOfSignatures;
}

void Payor::Status::setNumberOfSignatures(quint32 numberOfSignatures) {
    _numberOfSignatures = numberOfSignatures;
}

/**
 * Payor::Configuration
 */



/**
 * Payor
 */

#include "extension/BitCoin/P2SHTxOut.hpp"
#include "extension/BitCoin/BitSwaprjs.hpp"

Payor::Payor() {
}

/*
Payor::Payor(const Payor::Configuration & configuration) {

}
*/

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

quint32 Payor::addChannel(const Channel::PayorSettings & configuration) {
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

quint32 Payor::assignUnassignedSlot(const Channel::PayeeSettings & configuration) {
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
    _channels[index].setState(Channel::State::unassigned);

    // If some slots had refund signed state, i.e. valid signature, then revert state
    for(QVector<Channel>::iterator i = _channels.begin(),
            end(_channels.end()); i != end;i++) {

        // Get slot
        Channel & s = *i;

        // Reset state back to simply assigned, if a valid signature existed
        if(s.state() == Channel::State::refund_signed)
            s.setState(Channel::State::assigned);
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
    bool validSignature = BitSwaprjs::check_refund_signatures(OutPoint(_contractHash, index),
                                                             s.payorRefundSignature(),
                                                             signature,
                                                             s.payorContractKeyPair().pk(),
                                                             s.payeeContractPk(),
                                                             P2PKHTxOut(s.funds(), s.payorFinalKeyPair().pk()),
                                                             s.refundLockTime());

    // If it matched, then alter state and save signature
    if(validSignature) {

        s.setState(Channel::State::refund_signed);
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
    const Channel & channel = _channels[index];

    Q_ASSERT(channel.state() == Channel::State::refund_signed);

    // The amount paid so far
    quint64 amountPaid = channel.price()*channel.numberOfPaymentsMade();

    return BitSwaprjs::compute_payor_payment_signature(OutPoint(_contractHash, index),
                                                       P2PKHTxOut(channel.funds() - amountPaid, channel.payorFinalKeyPair().pk()),
                                                       P2PKHTxOut(amountPaid, channel.payeeContractPk()),
                                                       channel.payorContractKeyPair().sk());
}

bool Payor::claimRefund(quint32 index) const {

    // Try to spend refund
    return false;
}

bool Payor::spent(quint32 index) const {

    // Check if cannel payment has been spent
    return false;
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

void Payor::setNumberOfSignatures(quint32 numberOfSignedSlots) {
    _numberOfSignatures = numberOfSignedSlots;
}

