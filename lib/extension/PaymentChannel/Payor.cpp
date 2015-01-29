#include "Payor.hpp"
#include "Refund.hpp"
#include "Payment.hpp"
#include "Contract.hpp"

#include "extension/BitCoin/BitSwaprjs.hpp"

Payor::Slot::PayorConfiguration::PayorConfiguration(quint64 funds, const KeyPair &contractKeyPair, const KeyPair &finalKeyPair, quint64 refundFee)
    : _funds(funds)
    , _contractKeyPair(contractKeyPair)
    , _finalKeyPair(finalKeyPair)
    , _refundFee(refundFee)
    , _refundLockTime(refundLockTime){
}

quint64 Payor::Slot::PayorConfiguration::funds() const {
    return _funds;
}

void Payor::Slot::PayorConfiguration::setFunds(const quint64 &funds) {
    _funds = funds;
}

KeyPair Payor::Slot::PayorConfiguration::contractKeyPair() const {
    return _contractKeyPair;
}

void Payor::Slot::PayorConfiguration::setContractKeyPair(const KeyPair &contractKeyPair) {
    _contractKeyPair = contractKeyPair;
}

KeyPair Payor::Slot::PayorConfiguration::finalKeyPair() const {
    return _finalKeyPair;
}

void Payor::Slot::PayorConfiguration::setFinalKeyPair(const KeyPair &finalKeyPair) {
    _finalKeyPair = finalKeyPair;
}

quint64 Payor::Slot::PayorConfiguration::refundFee() const {
    return _refundFee;
}

void Payor::Slot::PayorConfiguration::setRefundFee(const quint64 &refundFee) {
    _refundFee = refundFee;
}

quint32 Payor::Slot::PayorConfiguration::getRefundLockTime() const {
    return refundLockTime;
}

void Payor::Slot::PayorConfiguration::setRefundLockTime(const quint32 &value) {
    refundLockTime = value;
}

Payor::Slot::PayeeConfiguration::PayeeConfiguration(quint64 price, const PublicKey &contractPk, const PublicKey &finalPk, quint64 paymentFee)
    : _price(price)
    , _contractPk(contractPk)
    , _finalPk(finalPk)
    , _paymentFee(paymentFee) {

}

quint64 Payor::Slot::PayeeConfiguration::paymentFee() const {
    return _paymentFee;
}

void Payor::Slot::PayeeConfiguration::setPaymentFee(const quint64 &paymentFee) {
    _paymentFee = paymentFee;
}

PublicKey Payor::Slot::PayeeConfiguration::finalPk() const {
    return _finalPk;
}

void Payor::Slot::PayeeConfiguration::setFinalPk(const PublicKey &finalPk) {
    _finalPk = finalPk;
}

PublicKey Payor::Slot::PayeeConfiguration::contractPk() const {
    return _contractPk;
}

void Payor::Slot::PayeeConfiguration::setContractPk(const PublicKey &contractPk) {
    _contractPk = contractPk;
}

quint64 Payor::Slot::PayeeConfiguration::price() const {
    return _price;
}

void Payor::Slot::PayeeConfiguration::setPrice(const quint64 &price) {
    _price = price;
}

Payor::Slot::Slot() {
}

Payor::Slot::Slot(const Slot& slot) {
    _index = slot.index();
    _state = slot.state();
    _price = slot.price();
    _numberOfPaymentsMade = slot.numberOfPaymentsMade();
    _funds = slot.funds();
    _payorContractKeyPair = slot.payorContractKeyPair();
    _payeeContractPk = slot.payeeContractPk();
    _payeeFinalPk = slot.payeeFinalPk();
    _refund = slot.refund();
    _refundFee = slot.refundFee();
    _paymentFee = slot.paymentFee();
    _refundLockTime = slot.refundLockTime();
}

Payor::Slot & Payor::Slot::operator=(const Slot& rhs) {
    _index = rhs.index();
    _state = rhs.state();
    _price = rhs.price();
    _numberOfPaymentsMade = rhs.numberOfPaymentsMade();
    _funds = rhs.funds();
    _payorContractKeyPair = rhs.payorContractKeyPair();
    _payeeContractPk = rhs.payeeContractPk();
    _payeeFinalPk = rhs.payeeFinalPk();
    _refund = rhs.refund();
    _refundFee = rhs.refundFee();
    _paymentFee = rhs.paymentFee();
    _refundLockTime = rhs.refundLockTime();
    return *this;
}

Payor::Slot::Slot(quint32 index,
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
                                , _refund(refund)
                                , _refundFee(refundFee)
                                , _paymentFee(paymentFee) {
}

Refund Payor::Slot::refund(const Hash &contractHash) const {
    return Refund(OutputPoint(contractHash, _index),
                  P2PKHTxOut(_funds - _refundFee, _payorContractKeyPair.pk(), _payeeContractPk),
                  _refundLockTime);
}

Payment Payor::Slot::payment(const Hash &contractHash) const {

    // The amount paid so far
    quint64 amountPaid = _price*_numberOfPaymentsMade;

    return Payment(OutputPoint(contractHash, _index),
                   P2PKHTxOut(_funds - amountPaid, _payorContractKeyPair.pk()),
                   P2PKHTxOut(amountPaid - _paymentFee, _payorContractKeyPair.pk()));
}

Signature Payor::Slot::refundSignature(const Hash &contractHash) const {

    // Create refund
    Refund refund = refund(contractHash, _refundLockTime);

    /**
      QJsonObject raw = refund.rawTransaction();
      return _payorContractKeyPair.sk().sign(raw, sighash);
      */

    return Signature();
}

Signature Payor::Slot::paymentSignature(const Hash &contractHash) const {

    // Create pamynent
    Payment payment = payment(contractHash);

    /**
      QJsonObject raw = payment.rawTransaction();
      return _payorContractKeyPair.sk().sign(raw, sighash);
      */

    return Signature();

}

void Payor::Slot::paymentMade() {
    _numberOfPaymentsMade++;
}

Payor::Slot::State Payor::Slot::state() const {
    return _state;
}

void Payor::Slot::setState(const State &state) {
    _state = state;
}

quint64 Payor::Slot::numberOfPaymentsMade() const {
    return _numberOfPaymentsMade;
}

void Payor::Slot::setNumberOfPaymentsMade(const quint64 &numberOfPaymentsMade) {
    _numberOfPaymentsMade = numberOfPaymentsMade;
}

Signature Payor::Slot::refund() const {
    return _refund;
}

void Payor::Slot::setRefund(const Signature &refund) {
    _refund = refund;
}

quint64 Payor::Slot::paymentFee() const {
    return _paymentFee;
}

void Payor::Slot::setPaymentFee(const quint64 &paymentFee) {
    _paymentFee = paymentFee;
}

PublicKey PaymentChannelPayPaymentChannelPayoror::Slot::payeeFinalPk() const {
    return _payeeFinalPk;
}

void Payor::Slot::setPayeeFinalPk(const PublicKey &payeeFinalPk) {
    _payeeFinalPk = payeeFinalPk;
}


quint64 Payor::Slot::refundFee() const {
    return _refundFee;
}

void Payor::Slot::setRefundFee(const quint64 &refundFee) {
    _refundFee = refundFee;
}

quint32 Payor::Slot::index() const {
    return _index;
}

void Payor::Slot::setIndex(const quint32 &index) {
    _index = index;
}

PublicKey Payor::Slot::payeeFinalPk() const {
    return _payeeFinalPk;
}

void Payor::Slot::setPayeeFinalPk(const PublicKey &payeeFinalPk) {
    _payeeFinalPk = payeeFinalPk;
}

quint32 Payor::Slot::refundLockTime() const {
    return _refundLockTime;
}

void Payor::Slot::setRefundLockTime(const quint32 &refundLockTime) {
    _refundLockTime = refundLockTime;
}

PublicKey Payor::Slot::payeeContractPk() const {
    return _payeeContractPk;
}

void Payor::Slot::setPayeeContractPk(const PublicKey &payeeContractPk) {
    _payeeContractPk = payeeContractPk;
}

KeyPair Payor::Slot::payorContractKeyPair() const {
    return _payorContractKeyPair;
}

void Payor::Slot::setPayorContractKeyPair(const KeyPair &payorKeyPair) {
    _payorContractKeyPair = payorKeyPair;
}

quint32 Payor::Slot::index() const {
    return _index;
}

void Payor::Slot::setIndex(const quint32 &index) {
    _index = index;
}

quint64 Payor::Slot::price() const {
    return _price;
}

void Payor::Slot::setPrice(const quint64 &priceIncrement) {
    _price = priceIncrement;
}

quint64 Payor::Slot::funds() const {
    return _funds;
}

void Payor::Slot::setFunds(const quint64 &funds) {
    _funds = funds;
}

Payor::Payor() {
}

Payor::Payor(const QSet<Slot::PayorConfiguration> & configurations, const OutputPoint& fundingOutput, const KeyPair& fundingOutputKeyPair)
    : _state(State::waiting_for_full_set_of_sellers)
    , _fundingOutput(fundingOutput)
    , _fundingOutputKeyPair(fundingOutputKeyPair) {

    // Check that _fundingOutput
    // *exists
    // *is unspent
    // *has correct output script with correct sighash
    // *is controlled by _fundingOutputKeyPair
    // *has enough value

    quint32 index = 0;
    for(QSet<Slot::PayorConfiguration>::iterator i = configurations.begin(),
            end(configurations.end()); i != end;i++) {

        // Get configuration
        Slot::PayorConfiguration payorConfiguration = *i;

        // Create slot
        Slot s;
        s.index(index);
        s.state(Slot::State::unassigned);
        s.numberOfPaymentsMade(0);
        s.payorContractKeyPair(payorConfiguration.contractKeyPair());
        s.payorFinalKeyPair(payorConfiguration.finalKeyPair());
        s.refundFee(payorConfiguration.refundFee());
        s.funds(payorConfiguration.funds());

        // Add slot
        _slots.append(s);

        // Increment counter
        index++;
    }
}

quint32 Payor::assignUnassignedSlot(const Slot::PayeeConfiguration & configuration) {

    // Check payor is trying to find sellers
    if(_state != State::waiting_for_full_set_of_sellers)
        throw std::exception("State incompatile request, must be in waiting_for_full_set_of_sellers state.");

    // Find a slot
    quint32 numberUnassigned = 0;
    quint32 slotIndex = -1;
    for(quint32 i = 0;i < _slots.size();i++) {

        // Get reference to slot
        Slot & s = _slots[i];

        if(s.state() == Slot::State::unassigned) {

            // Count number of unassigned slots
            numberUnassigned++;

            // Pick first unassigned slot
            if(numberUnassigned == 0) {

                // And save payee configurations for slot
                s.price(configuration.price());
                s.payeeContractPk(configuration.contractPk());
                s.payeeFinalPk(configuration.finalPk());
                s.paymentFee(configuration.paymentFee());
                s.state(Slot::State::assigned);

                // and slot index
                slotIndex = i;
            }
        }
    }

    // If this was the last unasigned slot, then alter state
    if(numberUnassigned == 1) {
        _state = State::waiting_for_full_set_of_refund_signatures;

        // Generate contract and save hash
        Contract contract = contract();
        _contractHash = BitSwaprjs.compute_contract_hash(contract);
    }

    return slotIndex;
}

void Payor::unassignSlot(quint32 index) {

    if(_state != State::waiting_for_full_set_of_sellers && _state != State::waiting_for_full_set_of_refund_signatures)
        throw std::exception("State incompatile request, must be in State::waiting_for_full_set_of_sellers or State::waiting_for_full_set_of_refund_signatures state.");

    // Check that index is valid
    if(index >= _slots.size())
        throw std::exception("Invalid index.");

    // Update slot state
    _slots[index].state(Slot::State::unassigned);

    // If some slots had valid signatures
    for(QVector<Slot>::iterator i = _slots.begin(),
            end(_slots.end()); i != end;i++) {

        // Get slot
        Slot & s = *i;

        // Reset state back to simply assigned, if a valid signature existed
        if(s.state() == Slot::State::refund_signed)
            s.state(Slot::State::assigned);
    }

    // Set state of payor
    _state = State::waiting_for_full_set_of_sellers;
}

bool Payor::processRefundSignature(quint32 index, const Signature & signature) {

    // Check that refunds are being collected
    if(_state != State::waiting_for_full_set_of_refund_signatures)
        throw std::exception("State incompatile request, must be in State::waiting_for_full_set_of_refund_signatures state.");

    // Check that index is valid
    if(index >= _slots.size())
        throw std::exception("Invalid index.");

    // Get slot
    Slot & s = _slots[index];

    Q_ASSERT(s.state() == Slot::State::assigned);

    // Get refund
    Refund refund = s.refund(_);

    // Get payor refund

    // Check

    // If it matched



}

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

Refund Payor::refund(quint32 index) const {
    return _slots[index].refund(_contractHash);
}

Payment Payor::payment(quint32 index) const {
    return _slots[index].payment(_contractHash);
}

bool Payor::spent(quint32 index) const {

}

OutputPoint Payor::fundingOutput() const {
    return _fundingOutput;
}

void Payor::setFundingOutput(const OutputPoint &fundingOutput) {
    _fundingOutput = fundingOutput;
}
