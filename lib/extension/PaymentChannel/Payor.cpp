#include "Payor.hpp"
#include "Refund.hpp"
#include "Payment.hpp"
//#include "Contract.hpp"

#include "extension/BitCoin/P2SHTxOut.hpp"

#include "extension/BitCoin/BitSwaprjs.hpp"

Payor::Channel::PayorConfiguration::PayorConfiguration(quint64 funds, const KeyPair &contractKeyPair, const KeyPair &finalKeyPair, quint64 refundFee)
    : _funds(funds)
    , _contractKeyPair(contractKeyPair)
    , _finalKeyPair(finalKeyPair)
    , _refundFee(refundFee)
    , _refundLockTime(refundLockTime){
}

quint64 Payor::Channel::PayorConfiguration::funds() const {
    return _funds;
}

void Payor::Channel::PayorConfiguration::setFunds(const quint64 &funds) {
    _funds = funds;
}

KeyPair Payor::Channel::PayorConfiguration::contractKeyPair() const {
    return _contractKeyPair;
}

void Payor::Channel::PayorConfiguration::setContractKeyPair(const KeyPair &contractKeyPair) {
    _contractKeyPair = contractKeyPair;
}

KeyPair Payor::Channel::PayorConfiguration::finalKeyPair() const {
    return _finalKeyPair;
}

void Payor::Channel::PayorConfiguration::setFinalKeyPair(const KeyPair &finalKeyPair) {
    _finalKeyPair = finalKeyPair;
}

quint64 Payor::Channel::PayorConfiguration::refundFee() const {
    return _refundFee;
}

void Payor::Channel::PayorConfiguration::setRefundFee(const quint64 &refundFee) {
    _refundFee = refundFee;
}

quint32 Payor::Channel::PayorConfiguration::getRefundLockTime() const {
    return refundLockTime;
}

void Payor::Channel::PayorConfiguration::setRefundLockTime(const quint32 &value) {
    refundLockTime = value;
}

Payor::Channel::PayeeConfiguration::PayeeConfiguration(quint64 price, const PublicKey &contractPk, const PublicKey &finalPk, quint64 paymentFee)
    : _price(price)
    , _contractPk(contractPk)
    , _finalPk(finalPk)
    , _paymentFee(paymentFee) {

}

quint64 Payor::Channel::PayeeConfiguration::paymentFee() const {
    return _paymentFee;
}

void Payor::Channel::PayeeConfiguration::setPaymentFee(const quint64 &paymentFee) {
    _paymentFee = paymentFee;
}

PublicKey Payor::Channel::PayeeConfiguration::finalPk() const {
    return _finalPk;
}

void Payor::Channel::PayeeConfiguration::setFinalPk(const PublicKey &finalPk) {
    _finalPk = finalPk;
}

PublicKey Payor::Channel::PayeeConfiguration::contractPk() const {
    return _contractPk;
}

void Payor::Channel::PayeeConfiguration::setContractPk(const PublicKey &contractPk) {
    _contractPk = contractPk;
}

quint64 Payor::Channel::PayeeConfiguration::price() const {
    return _price;
}

void Payor::Channel::PayeeConfiguration::setPrice(const quint64 &price) {
    _price = price;
}

Payor::Channel::Channel() {
}

Payor::Channel::Channel(const Channel& slot) {
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

Payor::Channel & Payor::Channel::operator=(const Channel& rhs) {
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

Payor::Channel::Channel(quint32 index,
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
Refund Payor::Channel::refund(const Hash &contractHash) const {
    return Refund(OutputPoint(contractHash, _index),
                  P2PKHTxOut(_funds - _refundFee, _payorContractKeyPair.pk(), _payeeContractPk),
                  _refundLockTime);
}

Payment Payor::Channel::payment(const Hash &contractHash) const {

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
    OutputPoint contractOutputPoint(contractHash, _index);
    P2PKHTxOut refundOutput(_funds, _payorFinalKeyPair.pk());

    // remove PKs later, no reason we need them

    _payorRefundSignature = BitSwaprjs.compute_payor_refund_signature(contractOutputPoint, _payorContractKeyPair.sk(), _payorContractKeyPair.pk(), _payeeContractPk, refundOutput,_refundLockTime);
}

Signature Payor::Channel::paymentSignature(const Hash &contractHash) const {

    // Create pamynent
    Payment payment = payment(contractHash);

    /**
      QJsonObject raw = payment.rawTransaction();
      return _payorContractKeyPair.sk().sign(raw, sighash);
      */

    return Signature();
}

QJsonObject Payor::Channel::json() const {
    return QJsonObject {
                          {"funds", _funds},
                          {"firstPk", _payorContractKeyPair.pk().toString()},
                          {"secondPk", _payeeContractPk.toString()}
                       };
}

void Payor::Channel::paymentMade() {
    _numberOfPaymentsMade++;
}

Payor::Channel::State Payor::Channel::state() const {
    return _state;
}

void Payor::Channel::setState(const State &state) {
    _state = state;
}

quint64 Payor::Channel::numberOfPaymentsMade() const {
    return _numberOfPaymentsMade;
}

void Payor::Channel::setNumberOfPaymentsMade(const quint64 &numberOfPaymentsMade) {
    _numberOfPaymentsMade = numberOfPaymentsMade;
}

Signature Payor::Channel::refund() const {
    return _payeeRefundSignature;
}

void Payor::Channel::setRefund(const Signature &refund) {
    _payeeRefundSignature = refund;
}

quint64 Payor::Channel::paymentFee() const {
    return _paymentFee;
}

void Payor::Channel::setPaymentFee(const quint64 &paymentFee) {
    _paymentFee = paymentFee;
}

PublicKey Payor::Channel::payeeFinalPk() const {
    return _payeeFinalPk;
}

void Payor::Channel::setPayeeFinalPk(const PublicKey &payeeFinalPk) {
    _payeeFinalPk = payeeFinalPk;
}


quint64 Payor::Channel::refundFee() const {
    return _refundFee;
}

void Payor::Channel::setRefundFee(const quint64 &refundFee) {
    _refundFee = refundFee;
}

quint32 Payor::Channel::index() const {
    return _index;
}

void Payor::Channel::setIndex(const quint32 &index) {
    _index = index;
}

PublicKey Payor::Channel::payeeFinalPk() const {
    return _payeeFinalPk;
}

void Payor::Channel::setPayeeFinalPk(const PublicKey &payeeFinalPk) {
    _payeeFinalPk = payeeFinalPk;
}

quint32 Payor::Channel::refundLockTime() const {
    return _refundLockTime;
}

void Payor::Channel::setRefundLockTime(const quint32 &refundLockTime) {
    _refundLockTime = refundLockTime;
}

Signature Payor::Channel::payeeRefundSignature() const
{
    return _payeeRefundSignature;
}

void Payor::Channel::setPayeeRefundSignature(const Signature &payeeRefundSignature)
{
    _payeeRefundSignature = payeeRefundSignature;
}

Signature Payor::Channel::payorRefundSignature() const
{
    return _payorRefundSignature;
}

void Payor::Channel::setPayorRefundSignature(const Signature &payorRefundSignature)
{
    _payorRefundSignature = payorRefundSignature;
}

PublicKey Payor::Channel::payeeContractPk() const {
    return _payeeContractPk;
}

void Payor::Channel::setPayeeContractPk(const PublicKey &payeeContractPk) {
    _payeeContractPk = payeeContractPk;
}

KeyPair Payor::Channel::payorContractKeyPair() const {
    return _payorContractKeyPair;
}

void Payor::Channel::setPayorContractKeyPair(const KeyPair &payorKeyPair) {
    _payorContractKeyPair = payorKeyPair;
}

quint32 Payor::Channel::index() const {
    return _index;
}

void Payor::Channel::setIndex(const quint32 &index) {
    _index = index;
}

quint64 Payor::Channel::price() const {
    return _price;
}

void Payor::Channel::setPrice(const quint64 &priceIncrement) {
    _price = priceIncrement;
}

quint64 Payor::Channel::funds() const {
    return _funds;
}

void Payor::Channel::setFunds(const quint64 &funds) {
    _funds = funds;
}

Payor::Payor() {
}

Payor::Payor(const QSet<Channel::PayorConfiguration> & configurations, const OutputPoint& fundingOutput, const KeyPair& fundingOutputKeyPair)
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

        // Create channel
        Channel s;
        s.index(index);
        s.state(Channel::State::unassigned);
        s.numberOfPaymentsMade(0);
        s.payorContractKeyPair(payorConfiguration.contractKeyPair());
        s.payorFinalKeyPair(payorConfiguration.finalKeyPair());
        s.refundFee(payorConfiguration.refundFee());
        s.funds(payorConfiguration.funds());

        // Add slot
        _channels.append(s);

        // Increment counter
        index++;
    }
}

quint32 Payor::assignUnassignedSlot(const Channel::PayeeConfiguration & configuration) {

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
    _numberOfSignedSlots = 0;
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
    bool validSignature = BitSwaprjs.check_refund_signatures(OutputPoint(_contractHash, index),
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

        _numberOfSignedSlots++;

        // Check if they are all signed
        if(_numberOfSignedSlots == _channels.size()) {
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

    return BitSwaprjs.compute_payor_payment_signature(OutputPoint(_contractHash, index),
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

OutputPoint Payor::fundingOutput() const {
    return _fundingOutput;
}

void Payor::setFundingOutput(const OutputPoint &fundingOutput) {
    _fundingOutput = fundingOutput;
}

quint32 Payor::numberOfSignedSlots() const {
    return _numberOfSignedSlots;
}

void Payor::setNumberOfSignedSlots(const quint32 &numberOfSignedSlots) {
    _numberOfSignedSlots = numberOfSignedSlots;
}

