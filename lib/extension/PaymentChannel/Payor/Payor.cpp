#include "Payor.hpp"

/*
#include "Refund.hpp"
#include "Payment.hpp"
#include "Contract.hpp"
*/

#include "extension/BitCoin/P2SHTxOut.hpp"
#include "extension/BitCoin/BitSwaprjs.hpp"

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

