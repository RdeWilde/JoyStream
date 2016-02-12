/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <paymentchannel/Payor.hpp>
#include <paymentchannel/ChannelConfiguration.hpp>
#include <paymentchannel/Commitment.hpp>
#include <paymentchannel/Refund.hpp>
#include <paymentchannel/Settlement.hpp>
#include <common/RedeemScriptHash.hpp>
#include <common/MultisigScriptPubKey.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/TransactionSignature.hpp>
#include <common/Utilities.hpp> // DEFAULT_SEQUENCE_NUMBER
#include <common/Bitcoin.hpp> // TEMPORARY, JUST TO GET HARD CODED SETTLEMENT FEE


#include <CoinCore/CoinNodeData.h> // Coin::TxOut

#include <QtMath>
#include <QDebug>

namespace joystream {
namespace paymentchannel {

    Payor::Channel::Channel() {
    }

    Payor::Channel::Channel(const ChannelConfiguration & configuration)
        : Channel(configuration.index(),
                  configuration.state(),
                  configuration.price(),
                  configuration.numberOfPaymentsMade(),
                  configuration.funds(),
                  configuration.payorContractKeyPair(),
                  configuration.payorFinalKeyPair(),
                  configuration.payeeContractPk(),
                  configuration.payeeFinalPk(),
                  configuration.payorRefundSignature(),
                  configuration.payeeRefundSignature(),
                  configuration.refundFee(),
                  configuration.paymentFee(),
                  configuration.refundLockTime()){
    }

    Payor::Channel::Channel(quint32 index,
                            ChannelState state,
                            quint64 price,
                            quint64 numberOfPaymentsMade,
                            quint64 funds,
                            const Coin::KeyPair & payorContractKeyPair,
                            const Coin::KeyPair & payorFinalKeyPair,
                            const Coin::PublicKey & payeeContractPk,
                            const Coin::PublicKey & payeeFinalPk,
                            const Coin::Signature & payorRefundSignature,
                            const Coin::Signature & payeeRefundSignature,
                            quint64 refundFee,
                            quint64 paymentFee,
                            quint32 refundLockTime)
        : _index(index)
        , _state(state)
        , _price(price)
        , _numberOfPaymentsMade(numberOfPaymentsMade)
        , _funds(funds)
        , _payorContractKeyPair(payorContractKeyPair)
        , _payorFinalKeyPair(payorFinalKeyPair)
        , _payeeContractPk(payeeContractPk)
        , _payeeFinalPk(payeeFinalPk)
        , _payorRefundSignature(payorRefundSignature)
        , _payeeRefundSignature(payeeRefundSignature)
        , _refundFee(refundFee)
        , _paymentFee(paymentFee)
        , _refundLockTime(refundLockTime) {
    }

    Coin::typesafeOutPoint Payor::Channel::contractOutPoint(const Coin::TransactionId & contractTxId) const {
        return Coin::typesafeOutPoint(contractTxId, _index);
    }

    Commitment Payor::Channel::commitment() const {

        if(_state == ChannelState::unassigned)
            throw std::runtime_error("No valid commitment exists for channel.");

        return Commitment(_funds, _payorContractKeyPair.pk(), _payeeContractPk);
    }

    Refund Payor::Channel::refund(const Coin::TransactionId & contractTxId) const {

        if(_state == ChannelState::unassigned)
            throw std::runtime_error("No valid commitment exists for channel.");

        // *** no fee !!!
        return Refund(contractOutPoint(contractTxId),
                      commitment(),
                      Coin::Payment(_funds, _payorFinalKeyPair.pk().toPubKeyHash()),
                      _refundLockTime);
    }

    Settlement Payor::Channel::settlement(const Coin::TransactionId & contractTxId) const {

        if(_state == ChannelState::unassigned)
            throw std::runtime_error("No valid commitment exists for channel.");

        // The amount paid so far
        quint64 paid = _price*_numberOfPaymentsMade;

        Q_ASSERT(paid <= _funds);

        return Settlement::dustLimitAndFeeAwareSettlement(contractOutPoint(contractTxId),
                                                          commitment(),
                                                          _payorFinalKeyPair.pk().toPubKeyHash(),
                                                          _payeeFinalPk.toPubKeyHash(),
                                                          _funds,
                                                          paid,
                                                          PAYCHAN_SETTLEMENT_FEE);
    }

    /**
    Coin::Signature Payor::Channel::createPayorRefundSignature(const Coin::TransactionId & contractHash) const {

        // Check that channel has been assigned
        if(_state != State::assigned)
            throw std::runtime_error("State incompatile request, must be in assigned state.");

        // Create transaction for spening contract output
        Coin::Transaction refund = contractSpendingTransaction(contractHash, _funds, _refundLockTime);

        // Return signature
        return ts.sig();
    }


    Coin::Signature Payor::Channel::createPaymentSignature(const Coin::TransactionId & contractHash) const {

        // Check that channel has been assigned
        if(_state != State::assigned)
            throw std::runtime_error("State incompatile request, must be in assigned state.");

        // How much has been paid to payee
        quint64 paid = _numberOfPaymentsMade*_price;

        // Create transaction for spening contract output
        Coin::Transaction payment = contractSpendingTransaction(contractHash, _funds - paid);

        // Add payee output
        payment.addOutput(Coin::TxOut(paid, Coin::P2PKHScriptPubKey(_payeeFinalPk).serialize()));

        // Create payor signature
        Coin::TransactionSignature ts = _payorContractKeyPair.sk().sign(payment, 0, contractOutputScriptPubKey().serialize(), Coin::SigHashType::all);

        // Return signature
        return ts.sig();
    }

    bool Payor::Channel::validateRefundSignature(const Coin::TransactionId & contractHash, const Coin::Signature & payeeSig) const {

        // Check that channel has been assigned
        if(_state != State::assigned)
            throw std::runtime_error("State incompatible request, must be in assigned state.");

        // Create transaction for spending contract output
        Coin::Transaction refund = contractSpendingTransaction(contractHash, _funds);

        // Compute sighash
        uchar_vector sigHash = Coin::sighash(refund,
                                             0,
                                             contractOutputScriptPubKey().serialize(),
                                             Coin::SigHashType::all);

        // Verify that signature is valid for payee public key on sighash
        return _payeeContractPk.verify(sigHash, payeeSig);
    }
    */

    void Payor::Channel::increaseNumberOfPayments() {
        _numberOfPaymentsMade++;
    }

    quint32 Payor::Channel::index() const {
        return _index;
    }

    void Payor::Channel::setIndex(quint32 index) {
        _index = index;
    }

    ChannelState Payor::Channel::state() const {
        return _state;
    }

    void Payor::Channel::setState(ChannelState state) {
        _state = state;
    }

    quint64 Payor::Channel::price() const {
        return _price;
    }

    void Payor::Channel::setPrice(quint64 price) {
        _price = price;
    }

    quint64 Payor::Channel::numberOfPaymentsMade() const {
        return _numberOfPaymentsMade;
    }

    void Payor::Channel::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
        _numberOfPaymentsMade = numberOfPaymentsMade;
    }

    quint64 Payor::Channel::funds() const {
        return _funds;
    }

    void Payor::Channel::setFunds(quint64 funds) {
        _funds = funds;
    }

    Coin::KeyPair Payor::Channel::payorContractKeyPair() const {
        return _payorContractKeyPair;
    }

    void Payor::Channel::setPayorContractKeyPair(const Coin::KeyPair & payorContractKeyPair) {
        _payorContractKeyPair = payorContractKeyPair;
    }

    Coin::KeyPair Payor::Channel::payorFinalKeyPair() const {
        return _payorFinalKeyPair;
    }

    void Payor::Channel::setPayorFinalKeyPair(const Coin::KeyPair & payorFinalKeyPair) {
        _payorFinalKeyPair = payorFinalKeyPair;
    }

    Coin::PublicKey Payor::Channel::payeeContractPk() const {
        return _payeeContractPk;
    }

    void Payor::Channel::setPayeeContractPk(const Coin::PublicKey & payeeContractPk) {
        _payeeContractPk = payeeContractPk;
    }

    Coin::PublicKey Payor::Channel::payeeFinalPk() const {
        return _payeeFinalPk;
    }

    void Payor::Channel::setPayeeFinalPk(const Coin::PublicKey & payeeFinalPk) {
        _payeeFinalPk = payeeFinalPk;
    }

    Coin::Signature Payor::Channel::payorRefundSignature() const {
        return _payorRefundSignature;
    }

    void Payor::Channel::setPayorRefundSignature(const Coin::Signature & payorRefundSignature) {
        _payorRefundSignature = payorRefundSignature;
    }

    Coin::Signature Payor::Channel::payeeRefundSignature() const {
        return _payeeRefundSignature;
    }

    void Payor::Channel::setPayeeRefundSignature(const Coin::Signature & payeeRefundSignature) {
        _payeeRefundSignature = payeeRefundSignature;
    }

    quint64 Payor::Channel::refundFee() const {
        return _refundFee;
    }

    void Payor::Channel::setRefundFee(quint64 refundFee) {
        _refundFee = refundFee;
    }

    quint64 Payor::Channel::paymentFee() const {
        return _paymentFee;
    }

    void Payor::Channel::setPaymentFee(quint64 paymentFee) {
        _paymentFee = paymentFee;
    }

    quint32 Payor::Channel::refundLockTime() const {
        return _refundLockTime;
    }

    void Payor::Channel::setRefundLockTime(quint32 refundLockTime) {
        _refundLockTime = refundLockTime;
    }

}
}

/**
 * Payor
 */

#include <paymentchannel/PayorConfiguration.hpp>
#include <paymentchannel/Commitment.hpp>
#include <paymentchannel/Contract.hpp>
#include <paymentchannel/Refund.hpp>
#include <common/Utilities.hpp> // DEFAULT_SEQUENCE_NUMBER
#include <common/TransactionSignature.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/P2PKHScriptSig.hpp>

#include <QVector>

namespace joystream {
namespace paymentchannel {

    Payor::Payor() {
    }

    Payor::Payor(const PayorConfiguration & configuration)
        : _state(configuration.state())
        , _utxo(configuration.utxo())
        //, _fundingOutPoint(configuration.fundingOutPoint())
        //, _fundingValue(configuration.fundingValue())
        //, _fundingOutputKeyPair(configuration.fundingOutputKeyPair())
        , _changeOutputKeyPair(configuration.changeOutputKeyPair())
        , _changeValue(configuration.changeValue())
        , _contractFee(configuration.contractFee())
        //, _contractTxId(configuration.contractHash())
        , _numberOfSignatures(configuration.numberOfSignatures()) {

        // Populate _channels vector
        const std::vector<ChannelConfiguration> & channelConfigurations = configuration.channels();

        // Counter for net output value, used to check integrity of fee
        quint64 netOutputValue = _changeValue;

        for(std::vector<ChannelConfiguration>::const_iterator i = channelConfigurations.cbegin(),
            end = channelConfigurations.cend(); i != end;i++) {

            // Get configuration
            const ChannelConfiguration channelConfiguration(*i);

            // Count funds
            netOutputValue += channelConfiguration.funds();

            // Create channel
            _channels.push_back(Channel(channelConfiguration));
        }

        if(_contractFee != _utxo.value() - netOutputValue)
            throw std::runtime_error("input, outputs, change and fee does not match.");
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

        // Get channel
        const Channel & channel = _channels[index];

        Q_ASSERT(channel.state() == ChannelState::refund_signed);

        // Get settelemnt
        Coin::TransactionId contractTxId = Coin::TransactionId::fromTx(_contractTx);
        Settlement settlement = channel.settlement(contractTxId);

        // Generate signature
        Coin::TransactionSignature settlementSignature = settlement.transactionSignature(channel.payorContractKeyPair().sk());

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

    quint64 Payor::minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB) {
        return PAYCHAN_SETTLEMENT_FEE*numberOfSellers + (maxPrice*numberOfSellers)*numberOfPiecesInTorrent + computeContractFee(numberOfSellers, feePerkB);
    }

    PayorState Payor::state() const {
        return _state;
    }

    void Payor::setState(PayorState state) {
        _state = state;
    }

    std::vector<Payor::Channel> & Payor::channels() {
        return _channels;
    }

    const Payor::Channel & Payor::channel(int i) const {
        Q_ASSERT(i >= 0 && i < _channels.size());
        return _channels[i];
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
