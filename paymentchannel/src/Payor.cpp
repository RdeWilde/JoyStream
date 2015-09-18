/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <paymentchannel/Payor.hpp>

/**
* Payor::Channel::Configuration
*/

Payor::Channel::Configuration::Configuration() {
}

Payor::Channel::Configuration::Configuration(quint32 index,
                                        State state,
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

quint32 Payor::Channel::Configuration::index() const {
    return _index;
}

void Payor::Channel::Configuration::setIndex(quint32 index) {
    _index = index;
}

Payor::Channel::State Payor::Channel::Configuration::state() const {
    return _state;
}

void Payor::Channel::Configuration::setState(State state) {
    _state =  state;
}

quint64 Payor::Channel::Configuration::price() const {
    return _price;
}

void Payor::Channel::Configuration::setPrice(quint64 price) {
    _price = price;
}

quint64 Payor::Channel::Configuration::numberOfPaymentsMade() const {
    return _numberOfPaymentsMade;
}

void Payor::Channel::Configuration::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
    _numberOfPaymentsMade = numberOfPaymentsMade;
}

quint64 Payor::Channel::Configuration::funds() const {
    return _funds;
}

void Payor::Channel::Configuration::setFunds(quint64 funds) {
    _funds = funds;
}

Coin::KeyPair Payor::Channel::Configuration::payorContractKeyPair() const {
    return _payorContractKeyPair;
}

void Payor::Channel::Configuration::setPayorContractKeyPair(const Coin::KeyPair & payorContractKeyPair) {
    _payorContractKeyPair = payorContractKeyPair;
}

Coin::KeyPair Payor::Channel::Configuration::payorFinalKeyPair() const {
    return _payorFinalKeyPair;
}

void Payor::Channel::Configuration::setPayorFinalKeyPair(const Coin::KeyPair & payorFinalKeyPair) {
    _payorFinalKeyPair = payorFinalKeyPair;
}

Coin::PublicKey Payor::Channel::Configuration::payeeContractPk() const {
    return _payeeContractPk;
}

void Payor::Channel::Configuration::setPayeeContractPk(const Coin::PublicKey & payeeContractPk) {
    _payeeContractPk = payeeContractPk;
}

Coin::PublicKey Payor::Channel::Configuration::payeeFinalPk() const {
    return _payeeFinalPk;
}

void Payor::Channel::Configuration::setPayeeFinalPk(const Coin::PublicKey & payeeFinalPk) {
    _payeeFinalPk = payeeFinalPk;
}

Coin::Signature Payor::Channel::Configuration::payorRefundSignature() const {
    return _payorRefundSignature;
}

void Payor::Channel::Configuration::setPayorRefundSignature(const Coin::Signature & payorRefundSignature) {
    _payorRefundSignature = payorRefundSignature;
}

Coin::Signature Payor::Channel::Configuration::payeeRefundSignature() const {
    return _payeeRefundSignature;
}

void Payor::Channel::Configuration::setPayeeRefundSignature(const Coin::Signature & payeeRefundSignature) {
    _payeeRefundSignature = payeeRefundSignature;
}

quint64 Payor::Channel::Configuration::refundFee() const {
    return _refundFee;
}

void Payor::Channel::Configuration::setRefundFee(quint64 refundFee) {
    _refundFee = refundFee;
}

quint64 Payor::Channel::Configuration::paymentFee() const {
    return _paymentFee;
}

void Payor::Channel::Configuration::setPaymentFee(quint64 paymentFee) {
    _paymentFee = paymentFee;
}

quint32 Payor::Channel::Configuration::refundLockTime() const {
    return _refundLockTime;
}

void Payor::Channel::Configuration::setRefundLockTime(quint32 refundLockTime) {
    _refundLockTime = refundLockTime;
}

/**
 * Payor::Channel::Status
 */

Payor::Channel::Status::Status() {

}

Payor::Channel::Status::Status(quint32 index,
                               State state,
                               quint64 price,
                               quint64 numberOfPaymentsMade,
                               quint64 funds,
                               quint32 refundLockTime)
    : _index(index)
    , _state(state)
    , _price(price)
    , _numberOfPaymentsMade(numberOfPaymentsMade)
    , _funds(funds)
    , _refundLockTime(refundLockTime) {
}

quint32 Payor::Channel::Status::refundLockTime() const {
    return _refundLockTime;
}

void Payor::Channel::Status::setRefundLockTime(quint32 refundLockTime) {
    _refundLockTime = refundLockTime;
}

quint64 Payor::Channel::Status::numberOfPaymentsMade() const {
    return _numberOfPaymentsMade;
}

void Payor::Channel::Status::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
    _numberOfPaymentsMade = numberOfPaymentsMade;
}

quint64 Payor::Channel::Status::funds() const {
    return _funds;
}

void Payor::Channel::Status::setFunds(quint64 funds) {
    _funds = funds;
}

quint64 Payor::Channel::Status::price() const {
    return _price;
}

void Payor::Channel::Status::setPrice(quint64 price) {
    _price = price;
}

Payor::Channel::State Payor::Channel::Status::state() const {
    return _state;
}

void Payor::Channel::Status::setState(State state) {
    _state = state;
}

quint32 Payor::Channel::Status::index() const {
    return _index;
}

void Payor::Channel::Status::setIndex(quint32 index) {
    _index = index;
}

/**
 * Payor::Channel
 */

#include <common/RedeemScriptHash.hpp>
#include <common/MultisigScriptPubKey.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/TransactionSignature.hpp>
#include <common/Utilities.hpp> // DEFAULT_SEQUENCE_NUMBER
#include <paymentchannel/Commitment.hpp>
#include <paymentchannel/Refund.hpp>
#include <paymentchannel/Settlement.hpp>

#include <CoinCore/CoinNodeData.h> // Coin::TxOut

#include <QDebug>

Payor::Channel::Channel() {
}

Payor::Channel::Channel(const Channel::Configuration & configuration)
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
                        const State & state,
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

    if(_state == State::unassigned)
        throw std::runtime_error("No valid commitment exists for channel.");

    return Commitment(_funds, _payorContractKeyPair.pk(), _payeeContractPk);
}

Refund Payor::Channel::refund(const Coin::TransactionId & contractTxId) const {

    if(_state == State::unassigned)
        throw std::runtime_error("No valid commitment exists for channel.");

    // *** no fee !!!
    return Refund(contractOutPoint(contractTxId),
                  commitment(),
                  Coin::Payment(_funds, _payorFinalKeyPair.pk().toPubKeyHash()),
                  _refundLockTime);
}

Settlement Payor::Channel::settlement(const Coin::TransactionId & contractTxId) const {

    if(_state == State::unassigned)
        throw std::runtime_error("No valid commitment exists for channel.");

    // The amount paid so far
    quint64 amountPaid = _price*_numberOfPaymentsMade;

    // *** no fee !!!
    return Settlement(contractOutPoint(contractTxId),
                      commitment(),
                      Coin::Payment(_funds - amountPaid, _payorFinalKeyPair.pk().toPubKeyHash()),
                      Coin::Payment(amountPaid, _payeeFinalPk.toPubKeyHash()));
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

Payor::Channel::Status Payor::Channel::status() const {

    return Payor::Channel::Status(_index,
                                  _state,
                                  _price,
                                  _numberOfPaymentsMade,
                                  _funds,
                                  _refundLockTime);
}

quint32 Payor::Channel::index() const {
    return _index;
}

void Payor::Channel::setIndex(quint32 index) {
    _index = index;
}

Payor::Channel::State Payor::Channel::state() const {
    return _state;
}

void Payor::Channel::setState(State state) {
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

/**
 * Payor::Status
 */

Payor::Status::Status() {

}

Payor::Status::Status(const std::vector<Channel::Status> & channels,
                      State state,
                      const Coin::UnspentP2PKHOutput & utxo,
                      quint64 changeValue,
                      quint64 contractFee,
                      const Coin::TransactionId & contractTxId,
                      quint32 numberOfSignatures)
    : _channels(channels)
    , _state(state)
    , _utxo(utxo)
    , _changeValue(changeValue)
    , _contractFee(contractFee)
    , _contractTxId(contractTxId)
    , _numberOfSignatures(numberOfSignatures) {
}

std::vector<Payor::Channel::Status> Payor::Status::channels() const {
    return _channels;
}

void Payor::Status::setChannels(const std::vector<Channel::Status> & channels) {
    _channels = channels;
}

Payor::State Payor::Status::state() const {
    return _state;
}

void Payor::Status::setState(State state) {
    _state = state;
}

Coin::UnspentP2PKHOutput Payor::Status::utxo() const {
    return _utxo;
}

void Payor::Status::setUtxo(const Coin::UnspentP2PKHOutput & utxo) {
    _utxo = utxo;
}

quint64 Payor::Status::changeValue() const {
    return _changeValue;
}

void Payor::Status::setChangeValue(quint64 changeValue) {
    _changeValue = changeValue;
}

quint64 Payor::Status::contractFee() const {
    return _contractFee;
}

void Payor::Status::setContractFee(quint64 contractFee) {
    _contractFee = contractFee;
}

Coin::TransactionId Payor::Status::contractTxId() const {
    return _contractTxId;
}

void Payor::Status::setContractTxId(const Coin::TransactionId & contractTxId) {
    _contractTxId = contractTxId;
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

Payor::Configuration::Configuration() {

}

Payor::Configuration::Configuration(Coin::Network network,
                                    State state,
                                    const std::vector<Channel::Configuration> & channels,
                                    const Coin::UnspentP2PKHOutput & utxo,
                                    //const OutPoint & fundingOutPoint,
                                    //quint64 fundingValue,
                                    //const KeyPair & fundingOutputKeyPair,
                                    const Coin::KeyPair & changeOutputKeyPair,
                                    quint64 changeValue,
                                    quint64 contractFee,
                                    const Coin::TransactionId & contractHash,
                                    quint32 numberOfSignatures)
    : _network(network)
    , _state(state)
    , _channels(channels)
    //, _fundingOutPoint(fundingOutPoint)
    //, _fundingValue(fundingValue)
    //, _fundingOutputKeyPair(fundingOutputKeyPair)
    , _utxo(utxo)
    , _changeOutputKeyPair(changeOutputKeyPair)
    , _changeValue(changeValue)
    , _contractFee(contractFee)
    , _contractHash(contractHash)
    , _numberOfSignatures(numberOfSignatures) {
}

Payor::State Payor::Configuration::state() const {
    return _state;
}

void Payor::Configuration::setState(State state) {
    _state = state;
}

std::vector<Payor::Channel::Configuration> Payor::Configuration::channels() const {
    return _channels;
}

void Payor::Configuration::setChannels(const std::vector<Payor::Channel::Configuration> & channels) {
    _channels = channels;
}

Coin::UnspentP2PKHOutput Payor::Configuration::utxo() const {
    return _utxo;
}

void Payor::Configuration::setUtxo(const Coin::UnspentP2PKHOutput & utxo) {
    _utxo = utxo;
}

/**
OutPoint Payor::Configuration::fundingOutPoint() const {
    return _fundingOutPoint;
}

void Payor::Configuration::setFundingOutPoint(const OutPoint & fundingOutPoint) {
    _fundingOutPoint = fundingOutPoint;
}

quint64 Payor::Configuration::fundingValue() const {
    return _fundingValue;
}

void Payor::Configuration::setFundingValue(quint64 fundingValue) {
    _fundingValue = fundingValue;
}

KeyPair Payor::Configuration::fundingOutputKeyPair() const {
    return _fundingOutputKeyPair;
}

void Payor::Configuration::setFundingOutputKeyPair(const KeyPair & fundingOutputKeyPair) {
    _fundingOutputKeyPair = fundingOutputKeyPair;
}
*/

Coin::KeyPair Payor::Configuration::changeOutputKeyPair() const {
    return _changeOutputKeyPair;
}

void Payor::Configuration::setChangeOutputKeyPair(const Coin::KeyPair & changeOutputKeyPair) {
    _changeOutputKeyPair = changeOutputKeyPair;
}

quint64 Payor::Configuration::changeValue() const {
    return _changeValue;
}

void Payor::Configuration::setChangeValue(quint64 changeValue) {
    _changeValue = changeValue;
}

quint64 Payor::Configuration::contractFee() const {
    return _contractFee;
}

void Payor::Configuration::setContractFee(quint64 contractFee) {
    _contractFee = contractFee;
}

Coin::TransactionId Payor::Configuration::contractHash() const {
    return _contractHash;
}

void Payor::Configuration::setContractHash(const Coin::TransactionId & contractHash) {
    _contractHash = contractHash;
}

quint32 Payor::Configuration::numberOfSignatures() const {
    return _numberOfSignatures;
}

void Payor::Configuration::setNumberOfSignatures(quint32 numberOfSignatures) {
    _numberOfSignatures = numberOfSignatures;
}

/**
 * Payor
 */

#include <paymentchannel/Commitment.hpp>
#include <paymentchannel/Contract.hpp>
#include <paymentchannel/Refund.hpp>

#include <common/Utilities.hpp> // DEFAULT_SEQUENCE_NUMBER

#include <common/TransactionSignature.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/P2PKHScriptSig.hpp>

#include <QVector>

Payor::Payor() {
}

Payor::Payor(const Payor::Configuration & configuration)
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
    const std::vector<Channel::Configuration> & channelConfigurations = configuration.channels();

    // Counter for net output value, used to check integrity of fee
    quint64 netOutputValue = _changeValue;

    for(std::vector<Channel::Configuration>::const_iterator i = channelConfigurations.cbegin(),
        end = channelConfigurations.cend(); i != end;i++) {

        // Get configuration
        const Channel::Configuration channelConfiguration(*i);

        // Count funds
        netOutputValue += channelConfiguration.funds();

        // Create channel
        _channels.push_back(Channel(channelConfiguration));
    }

    if(_contractFee != _utxo.value() - netOutputValue)
        throw std::runtime_error("input, outputs, change and fee does not match.");
}

quint32 Payor::assignUnassignedSlot(quint64 price, const Coin::PublicKey & payeeContractPk, const Coin::PublicKey & payeeFinalPk, quint32 refundLockTime) {

    // Check payor is trying to find sellers
    if(_state != State::waiting_for_full_set_of_sellers)
        throw std::runtime_error("State incompatile request, must be in waiting_for_full_set_of_sellers state.");

    // Find a slot which is unassigned,
    // and also count the total number of unassigned slots
    quint32 numberUnassigned = 0;
    quint32 slotIndex;
    bool slotAssigned = false;

    for(quint32 i = 0;i < _channels.size();i++) {

        // Get reference to slot
        Channel & s = _channels[i];

        if(s.state() == Channel::State::unassigned) {

            // If this is first unassigned slot we find,
            // then we pick it
            if(numberUnassigned == 0) {

                // And save payee configurations for slot
                s.setPrice(price);
                s.setPayeeContractPk(payeeContractPk);
                s.setPayeeFinalPk(payeeFinalPk);
                //s.paymentFee(configuration.paymentFee());
                s.setRefundLockTime(refundLockTime);
                s.setState(Channel::State::assigned);

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
        _state = State::waiting_for_full_set_of_refund_signatures;

        // Generate the contract transaction
        Contract c = contract();
        _contractTx = c.transaction();

        // Compute all refund signatures
        for(std::vector<Channel>::iterator i = _channels.begin(), end(_channels.end()); i != end;i++) {

            // Get refund
            Coin::TransactionId contractTxId(_contractTx);
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

    if(_state != State::waiting_for_full_set_of_sellers &&
       _state != State::waiting_for_full_set_of_refund_signatures)
        throw std::runtime_error("State incompatile request, must be in State::waiting_for_full_set_of_sellers or State::waiting_for_full_set_of_refund_signatures state.");

    // Check that index is valid
    if(index >= _channels.size())
        throw std::runtime_error("Invalid index.");

    // Update slot state
    _channels[index].setState(Channel::State::unassigned);

    // If some slots had refund signed state, i.e. valid signature, then revert state
    for(std::vector<Channel>::iterator i = _channels.begin(),
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

Contract Payor::contract() const {

    if(_state == State::waiting_for_full_set_of_sellers)
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
    if(_state != State::waiting_for_full_set_of_refund_signatures)
        throw std::runtime_error("State incompatile request, must be in State::waiting_for_full_set_of_refund_signatures state.");

    // Check that index is valid
    if(index >= _channels.size())
        throw std::runtime_error("Invalid index.");

    // Get channel
    Channel & channel = _channels[index];

    Q_ASSERT(channel.state() == Channel::State::assigned);

    // Check signature
    Coin::TransactionId contractTxId(_contractTx);
    bool validSignature = channel.refund(contractTxId).validate(channel.payeeContractPk(), signature);

    // If it matched, then alter state and save signature
    if(validSignature) {

        channel.setState(Channel::State::refund_signed);
        channel.setPayeeRefundSignature(signature);

        _numberOfSignatures++;

        // Check if they are all signed
        if(_numberOfSignatures == _channels.size()) {
            _state = State::can_pay;

            // What do we do now?!
            // up to user of channel to act.
        }
    }

    return validSignature;
}

quint64 Payor::incrementPaymentCounter(quint32 index) {

    // Check that we are paying
    if(_state != State::can_pay)
        throw std::runtime_error("State incompatile request, must be in State::paying state.");

    // Check that index is valid
    if(index >= _channels.size())
        throw std::runtime_error("Invalid index.");

    // Get channel
    Channel & channel = _channels[index];

    Q_ASSERT(channel.state() == Channel::State::refund_signed);

    // Update payment counter
    quint64 number = channel.numberOfPaymentsMade() + 1;
    channel.setNumberOfPaymentsMade(number);

    return number;
}

Coin::Signature Payor::getPresentPaymentSignature(quint32 index) const {

    // Check that we are paying
    if(_state != State::can_pay)
        throw std::runtime_error("State incompatile request, must be in State::paying state.");

    // Check that index is valid
    if(index >= _channels.size())
        throw std::runtime_error("Invalid index.");

    // Get channel
    const Channel & channel = _channels[index];

    Q_ASSERT(channel.state() == Channel::State::refund_signed);

    // Get settelemnt
    Coin::TransactionId contractTxId(_contractTx);
    Settlement settlement = channel.settlement(contractTxId);

    // Generate signature
    Coin::TransactionSignature settlementSignature = settlement.transactionSignature(channel.payorContractKeyPair().sk());

    return settlementSignature.sig();
}

Payor::Status Payor::status() const {

    // Get channel statuses
    std::vector<Channel::Status> channels;

    for(std::vector<Channel>::const_iterator i = _channels.begin(),
            end(_channels.end()); i != end;i++)
        channels.push_back(i->status());

    // Create rest of payor status
    Coin::TransactionId contractTxId(_contractTx);
    return Status(channels, _state, _utxo, _changeValue, _contractFee, contractTxId, _numberOfSignatures);
}

quint32 Payor::numberOfChannels() const {
    return _channels.size();
}

quint32 Payor::numberOfChannelsWithState(Channel::State state) const {

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
    return numberOfChannelsWithState(Channel::State::assigned) == _channels.size();
}

bool Payor::allRefundsSigned() const {
    return numberOfChannelsWithState(Channel::State::refund_signed) == _channels.size();
}

#include <QtMath>

quint64 Payor::computeContractFee(int numberOfSellers, quint64 feePerKb) {

    // Fee for contract based on fee estimate at http://bitcoinfees.com/
    // WE ADD ONE OUTPUT FOR THE CHANGE
    quint64 txByteSize =(148*1) + (34*(numberOfSellers + 1) + 10);
    quint64 fee = qCeil(feePerKb*((float)txByteSize/1000));

    return fee;
}

quint64 Payor::minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB) {
    return maxPrice*numberOfSellers*numberOfPiecesInTorrent + computeContractFee(numberOfSellers, feePerkB);
}

Payor::State Payor::state() const {
    return _state;
}

void Payor::setState(State state) {
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
