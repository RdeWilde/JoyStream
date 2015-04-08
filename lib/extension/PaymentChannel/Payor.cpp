#include "Payor.hpp"


// Payor::Channel::PayorSettings

Payor::Channel::PayorSettings::PayorSettings() {

}

Payor::Channel::PayorSettings::PayorSettings(quint64 funds, const PublicKey & contractPk, const PublicKey & finalPk)
    : _funds(funds)
    , _contractPk(contractPk)
    , _finalPk(finalPk) {
}

Payor::Channel::PayorSettings::PayorSettings(const PayorSettings & o) {
    *this = o;
}

PublicKey Payor::Channel::PayorSettings::finalPk() const {
    return _finalPk;
}

void Payor::Channel::PayorSettings::setFinalPk(const PublicKey & finalPk) {
    _finalPk = finalPk;
}

PublicKey Payor::Channel::PayorSettings::contractPk() const {
    return _contractPk;
}

void Payor::Channel::PayorSettings::setContractPk(const PublicKey & contractPk) {
    _contractPk = contractPk;
}

quint64 Payor::Channel::PayorSettings::funds() const {
    return _funds;
}

void Payor::Channel::PayorSettings::setFunds(quint64 funds) {
    _funds = funds;
}

Payor::Channel::PayorSettings & Payor::Channel::PayorSettings::operator=(const PayorSettings & o) {

    _funds = o.funds();
    _contractPk = o.contractPk(); //contractKeyPair();
    _finalPk = o.finalPk(); //finalKeyPair();

    return *this;
}

/**
// Payor::Channel::PayeeSettings

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
*/

/**
 * Payor::Channel::Configuration

Payor::Channel::Configuration::Configuration() {

}

Payor::Channel::Configuration::Configuration(quint32 index,
                                             quint64 funds,
                                             const KeyPair & payorContractKeyPair,
                                             const KeyPair & payorFinalKeyPair)
    : _index(index)
    , _state(State::unassigned)
    , _numberOfPaymentsMade(0)
    , _funds(funds)
    , _payorContractKeyPair(payorContractKeyPair)
    , _payorFinalKeyPair(payorFinalKeyPair){
}

quint32 Payor::Channel::Configuration::index() const {
    return _index;
}

void Payor::Channel::Configuration::setIndex(quint32 index) {
    _index = index;
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

QJsonObject Payor::Channel::json() const {
    return QJsonObject {
                {"funds", static_cast<qint64>(_funds)},
                {"firstPk", _payorContractKeyPair.pk().toString()},
                {"secondPk", _payeeContractPk.toString()}
            };
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
 * Payor::Channel::Status
 */

Payor::Channel::Status::Status() {

}

Payor::Channel::Status::Status(quint32 index,
                               State state,
                               quint64 price,
                               quint64 numberOfPaymentsMade,
                               quint32 refundLockTime)
    : _index(index)
    , _state(state)
    , _price(price)
    , _numberOfPaymentsMade(numberOfPaymentsMade)
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
 * Payor::Status
 */

Payor::Status::Status(const QVector<Channel::Status> & channels,
                      State state,
                      quint32 numberOfSignatures)
    : _channels(channels)
    , _state(state)
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

Payor::Configuration::Configuration(const QVector<Channel::PayorSettings> & channelSettings,
                                    const OutPoint & fundingOutPoint,
                                    const PublicKey & fundingOutPointPk,
                                    const PublicKey & changeOutPointPk,
                                    quint64 changeValue,
                                    quint64 maxPrice,
                                    quint32 maxLock)
    : _state(State::waiting_for_full_set_of_sellers)
    , _channels(channelSettings)
    , _fundingOutPoint(fundingOutPoint)
    , _fundingOutPointPk(fundingOutPointPk)
    , _changeOutPointPk(changeOutPointPk)
    , _changeValue(changeValue)
    , _maxPrice(maxPrice)
    , _maxLock(maxLock) {

    /**
    // Count number of sellers
    quint32 numberOfChannels = funds.count();

    // Generate two key pairs for each channel, the payors contract and final keys
    QList<KeyPair> payorContractKeyPairs = BitSwaprjs::generate_fresh_key_pairs(numberOfChannels);
    QList<KeyPair> payorFinalKeyPairs = BitSwaprjs::generate_fresh_key_pairs(numberOfChannels);

    // Create payor channel configurations, and add to vector
    for(quint32 index = 0; index < numberOfChannels;index++)
        _channels.push_back(Channel::Configuration(index,
                                                   funds[index],
                                                   payorContractKeyPairs[index],
                                                   payorFinalKeyPairs[index]));

    */
}

Payor::State Payor::Configuration::state() const {
    return _state;
}

void Payor::Configuration::setState(State state) {
    _state = state;
}

QVector<Payor::Channel::PayorSettings> Payor::Configuration::channels() const {
    return _channels;
}

void Payor::Configuration::setChannels(const QVector<Payor::Channel::PayorSettings> & channelSettings) {
    _channels = channelSettings;
}

quint32 Payor::Configuration::maxLock() const {
    return _maxLock;
}

void Payor::Configuration::setMaxLock(quint32 maxLock) {
    _maxLock = maxLock;
}

quint64 Payor::Configuration::maxPrice() const {
    return _maxPrice;
}

void Payor::Configuration::setMaxPrice(quint64 maxPrice) {
    _maxPrice = maxPrice;
}

quint64 Payor::Configuration::changeValue() const {
    return _changeValue;
}

void Payor::Configuration::setChangeValue(quint64 changeValue) {
    _changeValue = changeValue;
}

PublicKey Payor::Configuration::changeOutPointPk() const {
    return _changeOutPointPk;
}

void Payor::Configuration::setChangeOutPointPk(const PublicKey & changeOutPointPk) {
    _changeOutPointPk = changeOutPointPk;
}

PublicKey Payor::Configuration::fundingOutPointPk() const {
    return _fundingOutPointPk;
}

void Payor::Configuration::setFundingOutPointPk(const PublicKey & fundingOutPointPk) {
    _fundingOutPointPk = fundingOutPointPk;
}

OutPoint Payor::Configuration::fundingOutPoint() const {
    return _fundingOutPoint;
}

void Payor::Configuration::setFundingOutPoint(const OutPoint & fundingOutPoint) {
    _fundingOutPoint = fundingOutPoint;
}

/**
 * Payor
 */

#include "extension/BitCoin/P2SHTxOut.hpp"

#include "extension/BitCoin/Wallet.hpp"

//#include "extension/BitCoin/BitSwaprjs.hpp"

/*
Payor::Payor() {
}
*/

Payor::Payor(const Payor::Configuration & c)
    : _state(State::waiting_for_full_set_of_sellers)
    , _fundingOutPoint(c.fundingOutPoint())
    , _fundingOutputKeyPair(c.fundingOutPointPk(), _wallet->getSk(c.fundingOutPointPk()))
    , _changeOutputKeyPair(c.changeOutPointPk(), _wallet->getSk(c.changeOutPointPk()))
    , _changeValue(c.changeValue())
    , _maxPrice(c.maxPrice())
    , _maxLock(c.maxLock())
    , _numberOfSignatures(0){

    // Create channels
    quint32 index = 0;
    const QVector<Channel::PayorSettings> & settings = c.channels();
    for(QVector<Channel::PayorSettings>::const_iterator i = settings.constBegin();
        i != settings.constEnd();i++) {

        // Get settings for channel
        const Channel::PayorSettings & setting = *i;

        // Create channel
        Channel channel(index,
                        Channel::State::unassigned,
                        0,
                        0,
                        setting.funds(),
                        KeyPair(setting.contractPk(), _wallet->getSk(setting.contractPk())),
                        KeyPair(setting.finalPk(), _wallet->getSk(setting.finalPk())),
                        PublicKey(),
                        PublicKey(),
                        Signature(),
                        Signature(),
                        0,
                        0,
                        0);

        _channels.append(channel);

        index++;
    }
}

/*
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

quint32 Payor::addChannel(const Channel::PayorSettings & configuration) {

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
    return index + 1;

    return 0;
}
*/

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

            // Count number of unassigned slots
            numberUnassigned++;

            // Pick first unassigned slot
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
        }
    }

    // There must be an unassigned slot, given the _state check above.
    Q_ASSERT(slotIndex != -1);

    // If this was the last unasigned slot, then alter state
    if(numberUnassigned == 1) {

        // Update state to reflect that we are now waiting for signatures
        _state = State::waiting_for_full_set_of_refund_signatures;

        // Compute and set _contractHash
        _contractHash = BitSwaprjs::compute_contract_hash(_fundingOutPoint,
                                                          _fundingOutputKeyPair.sk(),
                                                          _channels,
                                                          P2PKHTxOut(_changeValue, _changeOutputKeyPair.pk()));

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
            _state = State::can_pay;

            // What do we do now?!
            // up to user of channel to act.
        }
    }

    return validSignature;
}

void Payor::broadcast_contract() {

    // Compute and set _contractHash
    BitSwaprjs::broadcast_contract(_fundingOutPoint,
                                  _fundingOutputKeyPair.sk(),
                                  _channels,
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

    return BitSwaprjs::compute_payment_signature(OutPoint(_contractHash, index),
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

Payor::State Payor::state() const {
    return _state;
}

void Payor::setState(State state) {
    _state = state;
}

QVector<Payor::Channel> & Payor::channels() {
    return _channels;
}

OutPoint Payor::fundingOutPoint() const {
    return _fundingOutPoint;
}

void Payor::setFundingOutPoint(const OutPoint &fundingOutput) {
    _fundingOutPoint = fundingOutput;
}

quint32 Payor::numberOfSignatures() const {
    return _numberOfSignatures;
}

void Payor::setNumberOfSignatures(quint32 numberOfSignedSlots) {
    _numberOfSignatures = numberOfSignedSlots;
}

quint64 Payor::maxPrice() const {
    return _maxPrice;
}

void Payor::setMaxPrice(quint64 maxPrice) {
    _maxPrice = maxPrice;
}

quint32 Payor::maxLock() const {
    return _maxLock;
}

void Payor::setMaxLock(quint32 maxLock) {
    _maxLock = maxLock;
}

TxId Payor::contractHash() const {
    return _contractHash;
}

void Payor::setContractHash(const TxId & contractHash) {
    _contractHash = contractHash;
}

/*
quint64 Payor::maxFeePerByte() const {
    return _maxFeePerByte;
}

void Payor::setMaxFeePerByte(quint64 maxFeePerByte) {
    _maxFeePerByte = maxFeePerByte;
}
*/
