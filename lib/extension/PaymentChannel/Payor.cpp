#include "Payor.hpp"

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
                                        const KeyPair & payorContractKeyPair,
                                        const KeyPair & payorFinalKeyPair,
                                        const PublicKey & payeeContractPk,
                                        const PublicKey & payeeFinalPk,
                                        const Signature & payorRefundSignature,
                                        const Signature & payeeRefundSignature,
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

KeyPair Payor::Channel::Configuration::payorContractKeyPair() const {
    return _payorContractKeyPair;
}

void Payor::Channel::Configuration::setPayorContractKeyPair(const KeyPair & payorContractKeyPair) {
    _payorContractKeyPair = payorContractKeyPair;
}

KeyPair Payor::Channel::Configuration::payorFinalKeyPair() const {
    return _payorFinalKeyPair;
}

void Payor::Channel::Configuration::setPayorFinalKeyPair(const KeyPair & payorFinalKeyPair) {
    _payorFinalKeyPair = payorFinalKeyPair;
}

PublicKey Payor::Channel::Configuration::payeeContractPk() const {
    return _payeeContractPk;
}

void Payor::Channel::Configuration::setPayeeContractPk(const PublicKey & payeeContractPk) {
    _payeeContractPk = payeeContractPk;
}

PublicKey Payor::Channel::Configuration::payeeFinalPk() const {
    return _payeeFinalPk;
}

void Payor::Channel::Configuration::setPayeeFinalPk(const PublicKey & payeeFinalPk) {
    _payeeFinalPk = payeeFinalPk;
}

Signature Payor::Channel::Configuration::payorRefundSignature() const {
    return _payorRefundSignature;
}

void Payor::Channel::Configuration::setPayorRefundSignature(const Signature & payorRefundSignature) {
    _payorRefundSignature = payorRefundSignature;
}

Signature Payor::Channel::Configuration::payeeRefundSignature() const {
    return _payeeRefundSignature;
}

void Payor::Channel::Configuration::setPayeeRefundSignature(const Signature & payeeRefundSignature) {
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

#include "extension/BitCoin/P2PKHTxOut.hpp"
#include "extension/BitCoin/BitSwaprjs.hpp"

#include <QJsonObject>

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
                        const KeyPair & payorContractKeyPair,
                        const KeyPair & payorFinalKeyPair,
                        const PublicKey & payeeContractPk,
                        const PublicKey & payeeFinalPk,
                        const Signature & payorRefundSignature,
                        const Signature & payeeRefundSignature,
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

void Payor::Channel::computePayorRefundSignature(const TxId &contractHash) {

    // Check that channel has been assigned
    if(_state != State::assigned)
        throw std::exception("State incompatile request, must be in assigned state.");

    // Make call to compute signature
    OutPoint contractOutPoint(contractHash, _index);
    P2PKHTxOut refundTxOut(_funds, _payorFinalKeyPair.pk());

    // remove PKs later, no reason we need them

    _payorRefundSignature = BitSwaprjs::compute_refund_signature(contractOutPoint,
                                                                      _payorContractKeyPair.sk(),
                                                                      _payorContractKeyPair.pk(),
                                                                      _payeeContractPk,
                                                                      refundTxOut,
                                                                      _refundLockTime);
}

Signature Payor::Channel::paymentSignature(const TxId &contractHash) const {

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

Payor::Channel::Status Payor::Channel::status() const {

    return Payor::Channel::Status(_index,
                                  _state,
                                  _price,
                                  _numberOfPaymentsMade,
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

void Payor::Channel::setState(const State & state) {
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

KeyPair Payor::Channel::payorContractKeyPair() const {
    return _payorContractKeyPair;
}

void Payor::Channel::setPayorContractKeyPair(const KeyPair & payorContractKeyPair) {
    _payorContractKeyPair = payorContractKeyPair;
}

KeyPair Payor::Channel::payorFinalKeyPair() const {
    return _payorFinalKeyPair;
}

void Payor::Channel::setPayorFinalKeyPair(const KeyPair & payorFinalKeyPair) {
    _payorFinalKeyPair = payorFinalKeyPair;
}

PublicKey Payor::Channel::payeeContractPk() const {
    return _payeeContractPk;
}

void Payor::Channel::setPayeeContractPk(const PublicKey & payeeContractPk) {
    _payeeContractPk = payeeContractPk;
}

PublicKey Payor::Channel::payeeFinalPk() const {
    return _payeeFinalPk;
}

void Payor::Channel::setPayeeFinalPk(const PublicKey & payeeFinalPk) {
    _payeeFinalPk = payeeFinalPk;
}

Signature Payor::Channel::payorRefundSignature() const {
    return _payorRefundSignature;
}

void Payor::Channel::setPayorRefundSignature(const Signature & payorRefundSignature) {
    _payorRefundSignature = payorRefundSignature;
}

Signature Payor::Channel::payeeRefundSignature() const {
    return _payeeRefundSignature;
}

void Payor::Channel::setPayeeRefundSignature(const Signature & payeeRefundSignature) {
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

Payor::Status::Status(const QVector<Channel::Status> & channels,
                      State state,
                      const UnspentP2PKHOutput & utxo,
                      const TxId & contractTxId,
                      quint32 numberOfSignatures)
    : _channels(channels)
    , _state(state)
    , _utxo(utxo)
    , _contractTxId(contractTxId)
    , _numberOfSignatures(numberOfSignatures) {
}

QVector<Payor::Channel::Status> Payor::Status::channels() const {
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

UnspentP2PKHOutput Payor::Status::utxo() const {
    return _utxo;
}

void Payor::Status::setUtxo(const UnspentP2PKHOutput & utxo) {
    _utxo = utxo;
}

TxId Payor::Status::contractTxId() const {
    return _contractTxId;
}

void Payor::Status::setContractTxId(const TxId &contractTxId) {
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

Payor::Configuration::Configuration(State state,
                                    const QVector<Channel::Configuration> & channels,
                                    const UnspentP2PKHOutput & utxo,
                                    //const OutPoint & fundingOutPoint,
                                    //quint64 fundingValue,
                                    //const KeyPair & fundingOutputKeyPair,
                                    const KeyPair & changeOutputKeyPair,
                                    quint64 changeValue,
                                    const TxId & contractHash,
                                    quint32 numberOfSignatures)
    : _state(state)
    , _channels(channels)
    //, _fundingOutPoint(fundingOutPoint)
    //, _fundingValue(fundingValue)
    //, _fundingOutputKeyPair(fundingOutputKeyPair)
    , _utxo(utxo)
    , _changeOutputKeyPair(changeOutputKeyPair)
    , _changeValue(changeValue)
    , _contractHash(contractHash)
    , _numberOfSignatures(numberOfSignatures) {
}

Payor::State Payor::Configuration::state() const {
    return _state;
}

void Payor::Configuration::setState(State state) {
    _state = state;
}

QVector<Payor::Channel::Configuration> Payor::Configuration::channels() const {
    return _channels;
}

void Payor::Configuration::setChannels(const QVector<Payor::Channel::Configuration> & channels) {
    _channels = channels;
}

UnspentP2PKHOutput Payor::Configuration::utxo() const {
    return _utxo;
}

void Payor::Configuration::setUtxo(const UnspentP2PKHOutput &utxo) {
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

KeyPair Payor::Configuration::changeOutputKeyPair() const {
    return _changeOutputKeyPair;
}

void Payor::Configuration::setChangeOutputKeyPair(const KeyPair & changeOutputKeyPair) {
    _changeOutputKeyPair = changeOutputKeyPair;
}

quint64 Payor::Configuration::changeValue() const {
    return _changeValue;
}

void Payor::Configuration::setChangeValue(quint64 changeValue) {
    _changeValue = changeValue;
}

TxId Payor::Configuration::contractHash() const {
    return _contractHash;
}

void Payor::Configuration::setContractHash(const TxId & contractHash) {
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

#include "extension/BitCoin/P2SHTxOut.hpp"

//#include "extension/BitCoin/Wallet.hpp"
//#include "extension/BitCoin/BitSwaprjs.hpp"

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
    , _contractTxId(configuration.contractHash())
    , _numberOfSignatures(configuration.numberOfSignatures()) {


    // Populate _channels vector
    const QVector<Channel::Configuration> & channelConfigurations = configuration.channels();

    for(QVector<Channel::Configuration>::const_iterator i = channelConfigurations.constBegin(),
        end(channelConfigurations.constEnd()); i != end;i++)
        _channels.append(Channel(*i));

}

quint32 Payor::assignUnassignedSlot(quint64 price, const PublicKey & payeeContractPk, const PublicKey & payeeFinalPk, quint32 refundLockTime) {

    // Check payor is trying to find sellers
    if(_state != State::waiting_for_full_set_of_sellers)
        throw std::exception("State incompatile request, must be in waiting_for_full_set_of_sellers state.");

    // Find a slot which is unassigned,
    // and also count the total number of unassigned slots
    quint32 numberUnassigned = 0;
    quint32 slotIndex = -1;

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
            }

            // Count number of unassigned slots
            numberUnassigned++;
        }
    }

    // There must be an unassigned slot, given the _state check above.
    Q_ASSERT(slotIndex != -1);

    // If this was the last unasigned slot, then alter state
    if(numberUnassigned == 1) {

        // Update state to reflect that we are now waiting for signatures
        _state = State::waiting_for_full_set_of_refund_signatures;

        // Build contract outputs
        QVector<P2SHTxOut> contractOutputs;

        for(QVector<Channel>::const_iterator i = _channels.constBegin(),
            end = _channels.constEnd();i != end;i++)
            contractOutputs.append(P2SHTxOut(i->funds(), i->payorContractKeyPair().pk(), i->payeeContractPk()));


        // Compute and set _contractHash
        _contractTxId = BitSwaprjs::compute_contract_hash(_utxo,
                                                          //_fundingOutPoint,
                                                          //_fundingValue,
                                                          //_fundingOutputKeyPair.sk(),
                                                          contractOutputs,
                                                          P2PKHTxOut(_changeValue, _changeOutputKeyPair.pk()));

        // Compute all refund signatures
        for(QVector<Channel>::Iterator i = _channels.begin(), end(_channels.end()); i != end;i++)
            i->computePayorRefundSignature(_contractTxId);
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
    bool validSignature = BitSwaprjs::check_refund_signatures(OutPoint(_contractTxId, index),
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
            _state = State::can_pay;

            // What do we do now?!
            // up to user of channel to act.
        }
    }

    return validSignature;
}

void Payor::broadcast_contract() {

    // Build contract outputs
    QVector<P2SHTxOut> contractOutputs;

    for(QVector<Channel>::const_iterator i = _channels.constBegin(),
        end = _channels.constEnd();i != end;i++)
        contractOutputs.append(P2SHTxOut(i->funds(), i->payorContractKeyPair().pk(), i->payeeContractPk()));

    // Compute and set _contractHash
    BitSwaprjs::broadcast_contract(//_fundingOutPoint,
                                  //_fundingOutputKeyPair.sk(),
                                  _utxo,
                                  contractOutputs,
                                  P2PKHTxOut(_changeValue, _changeOutputKeyPair.pk()));

}

quint64 Payor::incrementPaymentCounter(quint32 index) {

    // Check that we are paying
    if(_state != State::can_pay)
        throw std::exception("State incompatile request, must be in State::paying state.");

    // Check that index is valid
    if(index >= _channels.size())
        throw std::exception("Invalid index.");

    // Get channel
    Channel & channel = _channels[index];

    Q_ASSERT(channel.state() == Channel::State::refund_signed);

    // Update payment counter
    quint64 number = channel.numberOfPaymentsMade() + 1;
    channel.setNumberOfPaymentsMade(number);

    return number;
}

Signature Payor::getPresentPaymentSignature(quint32 index) const {

    // Check that we are paying
    if(_state != State::can_pay)
        throw std::exception("State incompatile request, must be in State::paying state.");

    // Check that index is valid
    if(index >= _channels.size())
        throw std::exception("Invalid index.");

    // Get channel
    const Channel & channel = _channels[index];

    Q_ASSERT(channel.state() == Channel::State::refund_signed);

    // The amount paid so far
    quint64 amountPaid = channel.price()*channel.numberOfPaymentsMade();

    return BitSwaprjs::compute_payment_signature(OutPoint(_contractTxId, index),
                                                 channel.payorContractKeyPair().sk(),
                                                 channel.payorContractKeyPair().pk(),
                                                 channel.payeeContractPk(),
                                                 P2PKHTxOut(channel.funds() - amountPaid, channel.payorFinalKeyPair().pk()),
                                                 P2PKHTxOut(amountPaid, channel.payeeContractPk()));
}

bool Payor::claimRefund(quint32 index) const {

    // Try to spend refund
    return false;
}

bool Payor::spent(quint32 index) const {

    // Check if cannel payment has been spent
    return false;
}

Payor::Status Payor::status() const {

    // Get channel statuses
    QVector<Channel::Status> channels;

    for(QVector<Channel>::const_iterator i = _channels.begin(),
            end(_channels.end()); i != end;i++)
        channels.push_back(i->status());

    // Create rest of payor status
    return Status(channels, _state, _numberOfSignatures);
}

quint32 Payor::numberOfChannels() const {
    return _channels.size();
}

quint32 Payor::numberOfChannelsWithState(Channel::State state) const {

    quint32 count = 0;
    for(QVector<Channel>::const_iterator i = _channels.constBegin(),
            end(_channels.constEnd()); i != end;i++) {

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

quint64 Payor::contractFee(int numberOfSellers, quint64 feePerKb) {

    // Fee for contract based on fee estimate at http://bitcoinfees.com/
    quint64 txByteSize =(148*1) + (34*numberOfSellers) + 10;
    quint64 fee = feePerKb*(txByteSize/1000);

    return fee;
}

quint64 Payor::minimalFunds(quint64 maxPrice, int numberOfSellers, quint64 feePerkB) {
    return maxPrice*numberOfSellers + contractFee(numberOfSellers, feePerkB);
}

Payor::State Payor::state() const {
    return _state;
}

void Payor::setState(State state) {
    _state = state;
}

QVector<Payor::Channel> & Payor::channels() {
    return _channels;
}

/**
OutPoint Payor::fundingOutPoint() const {
    return _fundingOutPoint;
}

void Payor::setFundingOutPoint(const OutPoint &fundingOutput) {
    _fundingOutPoint = fundingOutput;
}
*/

TxId Payor::contractHash() const {
    return _contractTxId;
}

void Payor::setContractHash(const TxId & contractHash) {
    _contractTxId = contractHash;
}

quint32 Payor::numberOfSignatures() const {
    return _numberOfSignatures;
}

void Payor::setNumberOfSignatures(quint32 numberOfSignatures) {
    _numberOfSignatures = numberOfSignatures;
}
