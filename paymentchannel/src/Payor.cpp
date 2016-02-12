/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <paymentchannel/Payor.hpp>
#include <paymentchannel/Contract.hpp>
#include <paymentchannel/Refund.hpp>
#include <paymentchannel/Settlement.hpp>
#include <common/TransactionSignature.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::TxOut

#include <QtMath> // Temproary, to do qCeil

namespace joystream {
namespace paymentchannel {

    Payor::Payor(Coin::Network network,
                 PayorState state,
                 const std::vector<Channel> & channels,
                 const Coin::UnspentP2PKHOutput & utxo,
                 const Coin::KeyPair & changeOutputKeyPair,
                 quint64 changeValue,
                 quint64 contractFee)
        : _network(network)
        , _state(state)
        , _channels(channels)
        , _utxo(utxo)
        , _changeOutputKeyPair(changeOutputKeyPair)
        , _changeValue(changeValue)
        , _contractFee(contractFee) {

        throw std::runtime_error("not done, derived fields are not set");
    }

    Payor Payor::unknownPayees() {

    }

    quint32 Payor::assignUnassignedSlot(quint64 price, const Coin::PublicKey & payeeContractPk, const Coin::PublicKey & payeeFinalPk, quint32 refundLockTime) {

        // Check payor is trying to find sellers
        if(_state != PayorState::waiting_for_full_set_of_sellers)
            throw std::runtime_error("State incompatile request, must be in waiting_for_full_set_of_sellers state.");

        // Find a slot which is unassigned,
        // and also count the total number of unassigned slots
        quint32 numberUnassigned = 0;
        quint32 slotIndex;
        bool slotAssigned = false;

        for(quint32 i = 0;i < _channels.size();i++) {

            // Get reference to slot
            Channel & s = _channels[i];

            if(s.state() == ChannelState::unassigned) {

                // If this is first unassigned slot we find,
                // then we pick it
                if(numberUnassigned == 0) {

                    // And save payee configurations for slot
                    s.setPrice(price);
                    s.setPayeeContractPk(payeeContractPk);
                    s.setPayeeFinalPk(payeeFinalPk);
                    //s.paymentFee(configuration.paymentFee());
                    s.setRefundLockTime(refundLockTime);
                    s.setState(ChannelState::assigned);

                    // and slot index
                    slotIndex = i;

                    // Remember that slot was assigned
                    slotAssigned = true;
                }

                // Count number of unassigned slots
                numberUnassigned++;
            }
        }

        // There must be an unassigned slot, given the _state check above.
        Q_ASSERT(slotAssigned);

        // If this was the last unasigned slot, then alter state
        if(numberUnassigned == 1) {

            // Update state to reflect that we are now waiting for signatures
            _state = PayorState::waiting_for_full_set_of_refund_signatures;

            // Generate the contract transaction
            Contract c = contract();
            _contractTx = c.transaction();

            // Compute all refund signatures
            for(std::vector<Channel>::iterator i = _channels.begin(), end(_channels.end()); i != end;i++) {

                // Get refund
                Coin::TransactionId contractTxId = Coin::TransactionId::fromTx(_contractTx);
                Refund refund = i->refund(contractTxId);

                // Get refund signature
                Coin::TransactionSignature refundSignature = refund.transactionSignature(i->payorContractKeyPair().sk());

                // Save signature in channel
                i->setPayorRefundSignature(refundSignature.sig());
            }
        }

        return slotIndex;
    }

    void Payor::unassignSlot(quint32 index) {

        if(_state != PayorState::waiting_for_full_set_of_sellers &&
           _state != PayorState::waiting_for_full_set_of_refund_signatures)
            throw std::runtime_error("State incompatile request, must be in State::waiting_for_full_set_of_sellers or State::waiting_for_full_set_of_refund_signatures state.");

        // Check that index is valid
        if(index >= _channels.size())
            throw std::runtime_error("Invalid index.");

        // Update slot state
        _channels[index].setState(ChannelState::unassigned);

        // If some slots had refund signed state, i.e. valid signature, then revert state
        for(std::vector<Channel>::iterator i = _channels.begin(),
                end(_channels.end()); i != end;i++) {

            // Get slot
            Channel & s = *i;

            // Reset state back to simply assigned, if a valid signature existed
            if(s.state() == ChannelState::refund_signed)
                s.setState(ChannelState::assigned);
        }

        // Set state of payor
        _state = PayorState::waiting_for_full_set_of_sellers;

        // No signature is now valid
        _numberOfSignatures = 0;
    }

    Contract Payor::contract() const {

        if(_state == PayorState::waiting_for_full_set_of_sellers)
            throw std::runtime_error("State incompatile request, no valid contract can be generated without a full set of sellers.");

        // Collect channel commitments
        std::vector<Commitment> commitments;

        for(std::vector<Channel>::const_iterator
            i =  _channels.cbegin(),
            end = _channels.cend();
            i != end;
            i++)
            commitments.push_back((*i).commitment());

        // Build contract
        return Contract(_utxo, commitments, Coin::Payment(_changeValue, _changeOutputKeyPair.pk().toPubKeyHash()));
    }

    /**
    Coin::Transaction Payor::contractTransaction() const {

        // Create transaction
        Coin::Transaction transaction;

        // Iterate channels and generate outputs
        for(int i = 0;i < _channels.size();i++)
            transaction.addOutput(_channels[i].contractOutput());

        // Add refund output
        transaction.addOutput(Coin::TxOut(_changeValue, Coin::P2PKHScriptPubKey(_changeOutputKeyPair.pk()).serialize()));

        // Add (unsigned) input spending funding utxo
        transaction.addInput(Coin::TxIn(_utxo.outPoint().getClassicOutPoint(), uchar_vector(), DEFAULT_SEQUENCE_NUMBER));

        // Creates spending input script
        setScriptSigToSpendP2PKH(transaction, 0, _utxo.keyPair().sk());

        return transaction;
    }
    */

    bool Payor::processRefundSignature(quint32 index, const Coin::Signature & signature) {

        // Check that refunds are being collected
        if(_state != PayorState::waiting_for_full_set_of_refund_signatures)
            throw std::runtime_error("State incompatile request, must be in State::waiting_for_full_set_of_refund_signatures state.");

        // Check that index is valid
        if(index >= _channels.size())
            throw std::runtime_error("Invalid index.");

        // Get channel
        Channel & channel = _channels[index];

        Q_ASSERT(channel.state() == ChannelState::assigned);

        // Check signature
        Coin::TransactionId contractTxId = Coin::TransactionId::fromTx(_contractTx);
        bool validSignature = channel.refund(contractTxId).validate(channel.payeeContractPk(), signature);

        // If it matched, then alter state and save signature
        if(validSignature) {

            channel.setState(ChannelState::refund_signed);
            channel.setPayeeRefundSignature(signature);

            _numberOfSignatures++;

            // Check if they are all signed
            if(_numberOfSignatures == _channels.size()) {
                _state = PayorState::can_pay;

                // What do we do now?!
                // up to user of channel to act.
            }
        }

        return validSignature;
    }

    quint64 Payor::incrementPaymentCounter(quint32 index) {

        // Check that we are paying
        if(_state != PayorState::can_pay)
            throw std::runtime_error("State incompatile request, must be in State::paying state.");

        // Check that index is valid
        if(index >= _channels.size())
            throw std::runtime_error("Invalid index.");

        // Get channel
        Channel & channel = _channels[index];

        Q_ASSERT(channel.state() == ChannelState::refund_signed);

        // Update payment counter
        quint64 number = channel.numberOfPaymentsMade() + 1;

        if(number * channel.price() > channel.funds())
            throw std::runtime_error("Channel has unsufficient funds to allow additional spending.");

        channel.setNumberOfPaymentsMade(number);

        return number;
    }

    Coin::Signature Payor::getPresentPaymentSignature(quint32 index) const {

        // Check that we are paying
        if(_state != PayorState::can_pay)
            throw std::runtime_error("State incompatile request, must be in State::paying state.");

        // Check that index is valid
        if(index >= _channels.size())
            throw std::runtime_error("Invalid index.");

        Q_ASSERT(_channels[index].state() == ChannelState::refund_signed);

        // Get settelemnt
        Coin::TransactionId contractTxId = Coin::TransactionId::fromTx(_contractTx);
        Settlement settlement = _channels[index].settlement(contractTxId, _settlementFee);

        // Generate signature
        Coin::TransactionSignature settlementSignature = settlement.transactionSignature(_channels[index].payorContractKeyPair().sk());

        return settlementSignature.sig();
    }

    quint32 Payor::numberOfChannels() const {
        return _channels.size();
    }

    quint32 Payor::numberOfChannelsWithState(ChannelState state) const {

        quint32 count = 0;
        for(std::vector<Channel>::const_iterator i = _channels.cbegin(),
                end = _channels.cend(); i != end;i++) {

            // Get channel
            const Channel & channel = *i;

            // Count channel if it has given state
            if(channel.state() == state)
                count++;
        }

        return count;
    }

    bool Payor::isFull() const {
        return numberOfChannelsWithState(ChannelState::assigned) == _channels.size();
    }

    bool Payor::allRefundsSigned() const {
        return numberOfChannelsWithState(ChannelState::refund_signed) == _channels.size();
    }

    quint64 Payor::computeContractFee(int numberOfSellers, quint64 feePerKb) {

        // Fee for contract based on fee estimate at http://bitcoinfees.com/
        // WE ADD ONE OUTPUT FOR THE CHANGE
        quint64 txByteSize =(148*1) + (34*(numberOfSellers + 1) + 10);
        quint64 fee = qCeil(feePerKb*((float)txByteSize/1000));

        return fee;
    }

    quint64 Payor::minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB, quint64 paychanSettlementFee) {
        return paychanSettlementFee*numberOfSellers + (maxPrice*numberOfSellers)*numberOfPiecesInTorrent + computeContractFee(numberOfSellers, feePerkB);
    }

    PayorState Payor::state() const {
        return _state;
    }

    void Payor::setState(PayorState state) {
        _state = state;
    }

    Coin::Transaction Payor::contractTransaction() const {

        /**
        Enforce state here some way?
        _state == State::waiting_for_full_set_of_sellers
        */

        return _contractTx;
    }

    /**
    OutPoint Payor::fundingOutPoint() const {
        return _fundingOutPoint;
    }

    void Payor::setFundingOutPoint(const OutPoint &fundingOutput) {
        _fundingOutPoint = fundingOutput;
    }
    */

    quint32 Payor::numberOfSignatures() const {
        return _numberOfSignatures;
    }

    void Payor::setNumberOfSignatures(quint32 numberOfSignatures) {
        _numberOfSignatures = numberOfSignatures;
    }

    quint64 Payor::contractFee() const {
        return _contractFee;
    }

}
}
