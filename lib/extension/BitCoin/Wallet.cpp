#include "Wallet.hpp"

#include <QJsonValue>
#include <QJsonObject>

#include "Utilities.hpp"

/**
 * Wallet::Entry::Output
 */

 Wallet::KeyInformation::Output::Output() {
 }

 Wallet::KeyInformation::Output::Output(const OutPoint & point,
                             const QDateTime & added,
                             const QString & description,
                             Type type,
                             State state,
                             quint64 value,
                             const Hash & blockHash,
                             quint32 blockHeight,
                             bool spent)
    : _point(point)
    , _added(added)
    , _description(description)
    , _type(type)
    , _state(state)
    , _value(value)
    , _blockHash(blockHash)
    , _blockHeight(blockHeight)
    , _spent(spent){
 }

 Wallet::KeyInformation::Output::Output(const QJsonObject & json) {

    // _point
    QJsonObject pointObject = Utilities::GET_OBJECT(json, "_point");
    _point = OutPoint(pointObject);

    // _added
    QString addedString = Utilities::GET_STRING(json, "_added");
    _added = QDateTime::fromString(addedString, Qt::ISODate);

    // _description
    _description = Utilities::GET_STRING(json, "_description");

    // _type
    QString type = Utilities::GET_STRING(json, "_type");

    if(type == "ContractChange")
        _type = Type::ContractChange;
    else if(type == "ContractOutputChange")
        _type = Type::ContractOutputChange;
    else if(type == "ContractOutputRefund")
        _type = Type::ContractOutputRefund;
    else if(type == "Payment")
        _type = Type::Payment;
    else if(type == "Receive")
        _type = Type::Receive;
    else
        throw new std::exception("_type key must have valud among {ContractChange, ContractOutputChange, ContractOutputRefund, Payment, Receive} of type QJsonValue::String.");

    // _value
    _value = Utilities::GET_DOUBLE(json, "_value");

    // _blockHash
    QString blockHashString = Utilities::GET_STRING(json, "_blockHash");
    _blockHash = Hash(blockHashString);

    // _blockHeight
    _blockHeight = Utilities::GET_DOUBLE(json, "_blockHeight");

    // _spent
    _spent = Utilities::GET_BOOL(json, "_spent");
 }

 bool Wallet::KeyInformation::Output::operator<(const Output & rhs) {
     return _point < rhs.point();
 }

 QJsonObject Wallet::KeyInformation::Output::toJson() const {

     QJsonObject json;

     json["_point"] = _point.toJson();
     json["_added"] = QJsonValue(_added.toString());
     json["_description"] = QJsonValue(_description);

     QString type;
     switch(_type) {

         case Type::ContractChange: type = "ContractChange"; break;
         case Type::ContractOutputChange: type = "ContractOutputChange"; break;
         case Type::ContractOutputRefund: type = "ContractOutputRefund"; break;
         case Type::Payment: type = "Payment"; break;
         case Type::Receive: type = "Receive"; break;
     }

     json["_type"] = QJsonValue(type);
     json["_value"] = QJsonValue(static_cast<double>(_value));
     json["_blockHash"] = QJsonValue(_blockHash.toString());
     json["_blockHeight"] = QJsonValue(static_cast<double>(_blockHeight));
     json["_spent"] = QJsonValue(_spent);

     return json;
 }

 OutPoint Wallet::KeyInformation::Output::point() const {
     return _point;
 }

 void Wallet::KeyInformation::Output::setPoint(const OutPoint & point) {
     _point = point;
 }

 QDateTime Wallet::KeyInformation::Output::added() const {
     return _added;
 }

 void Wallet::KeyInformation::Output::setAdded(const QDateTime & added) {
     _added = added;
 }

 QString Wallet::KeyInformation::Output::description() const {
     return _description;
 }

 void Wallet::KeyInformation::Output::setDescription(const QString & description) {
     _description = description;
 }

  Wallet::KeyInformation::Output::Type Wallet::KeyInformation::Output::type() const {
     return _type;
  }

  void Wallet::KeyInformation::Output::setType(Type type) {
      _type = type;
  }

  Wallet::KeyInformation::Output::State Wallet::KeyInformation::Output::state() const {
      return _state;
  }

  void Wallet::KeyInformation::Output::setState(State state) {
      _state = state;
  }

  quint64 Wallet::KeyInformation::Output::value() const {
      return _value;
  }

  void Wallet::KeyInformation::Output::setValue(quint64 value) {
      _value = value;
  }

  Hash Wallet::KeyInformation::Output::blockHash() const {
      return _blockHash;
  }

  void Wallet::KeyInformation::Output::setBlockHash(const Hash & blockHash) {
      _blockHash = blockHash;
  }

  quint32 Wallet::KeyInformation::Output::blockHeight() const {
      return _blockHeight;
  }

  void Wallet::KeyInformation::Output::setBlockHeight(quint32 blockHeight) {
      _blockHeight = blockHeight;
  }

  bool Wallet::KeyInformation::Output::spent() const {
      return _spent;
  }

  void Wallet::KeyInformation::Output::setSpent(bool spent) {
      _spent = spent;
  }

  /**
 * Wallet::Entry
 */

  Wallet::KeyInformation::KeyInformation() {

  }

  Wallet::KeyInformation::KeyInformation(const QDateTime & added, const QString & description, const QMap<OutPoint, Output> & outputs)
    : _added(added)
    , _description(description)
    , _outputs(outputs) {
  }

  Wallet::KeyInformation::KeyInformation(const QJsonObject & json) {

      // Parse _added
      QString addedString = Utilities::GET_STRING(json, "_added");

      _added = QDateTime::fromString(addedString, Qt::ISODate);

      // Parse _description
      _description = Utilities::GET_STRING(json, "_added");

      // Parse _outputs
      QJsonObject ouputsObject = Utilities::GET_OBJECT(json, "_outputs");

      for(QJsonObject::const_iterator i = ouputsObject.constBegin();
          i != ouputsObject.constEnd();i++) {

          OutPoint key(i.key());

          // Parse Output
          QJsonValue valueValue = i.value();

          if(valueValue.type() != QJsonValue::Object)
              throw new std::exception("value in _outputs is not of type QJsonValue::Object.");

          // Save mapping
          _outputs[key] = Output(valueValue.toObject());
      }
  }

  QJsonObject Wallet::KeyInformation::toJson() const {

      QJsonObject json;

      // _added
      json["_added"] = _added.toString(Qt::ISODate);

      // _description
      json["_description"] = QJsonValue(_description);

      // _outputs
      QJsonObject outputs;

      for(QMap<OutPoint, Output>::const_iterator i = _outputs.constBegin();
          i != _outputs.constEnd();i++)
          outputs[(i.key()).toString()] = Output(i.value()).toJson();

      json["_outputs"] = outputs;

      return json;
  }

  QDateTime Wallet::KeyInformation::added() const {
      return _added;
  }

  void Wallet::KeyInformation::setAdded(const QDateTime & added) {
      _added = added;
  }

  QString Wallet::KeyInformation::description() const {
      return _description;
  }

  void Wallet::KeyInformation::setDescription(const QString & description) {
      _description = description;
  }

  QMap<OutPoint, Wallet::KeyInformation::Output> Wallet::KeyInformation::outputs() const {
      return _outputs;
  }

  void Wallet::KeyInformation::setOutputs(const QMap<OutPoint, Wallet::KeyInformation::Output> & outputs) {
      _outputs = outputs;
  }

/**
 * Wallet
 */

#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>

  Wallet::Wallet(const QString & file, bool autoSave)
    : _walletFileName(file)
    , _autoSave(autoSave)
    , _walletFile(file){

      // Open wallet file
      if(!_walletFile.open(QIODevice::ReadWrite | QIODevice::Text))
          throw new std::exception("Could not open wallet file.");

      // Read entire file
      QByteArray walletRaw = _walletFile.readAll();

      /*************************************
       ***** DECRYPT HERE IN THE FUTURE ****
       *************************************/

      // Parse into json
      QJsonDocument doc = QJsonDocument::fromJson(walletRaw);
      QJsonObject walletDictionary = doc.object();

      // Parse out _entries
      QJsonObject entriesDictionary = Utilities::GET_OBJECT(walletDictionary, "_entries");

      for(QJsonObject::const_iterator i = entriesDictionary.constBegin();
          i != entriesDictionary.constEnd();i++) {

          // Parse key into PrivateKey
          QJsonValue privateKeyValue = i.key();

          if(privateKeyValue != QJsonValue::String)
              throw new std::exception("key in _entries not of type QJsonValue::String.");

          PrivateKey key(privateKeyValue.toString());

          // Parse into KeyInformation
          QJsonValue keyInformationValue = i.value();

          if(keyInformationValue.type() != QJsonValue::Object)
            throw new std::exception("value in _entries not of type QJsonValue::Object.");

          // Save mapping
          _entries[key] = KeyInformation(keyInformationValue.toObject());
      }

      // Parse out _entries
      QString chain = Utilities::GET_STRING(walletDictionary, "_chain");

      if(chain == "mainnet")
          _chain = Chain::mainnet;
      else if(chain == "testnet3")
          _chain = Chain::testnet3;
      else
          throw new std::exception("_chain key must have value among {mainnet, testnet3}.");
  }

  QJsonObject Wallet::toJson() {

      QJsonObject wallet;

      _mutex.lock();

      // Jsonify _entries
      QJsonObject entriesObject;

      for(QMap<PrivateKey, KeyInformation>::const_iterator i = _entries.constBegin();
          i != _entries.constEnd();i++)
        entriesObject[(i.key()).toString()] = (i.value()).toJson();

      wallet["_entries"] = entriesObject;

      // Jsonify _chain
      if(_chain == Chain::mainnet)
          wallet["_chain"] = "mainnet";
      else
          wallet["_chain"] = "testnet3";

      _mutex.unlock();

      return wallet;
  }

  void Wallet::save() {

      _mutex.lock();

      // Turn wallet into raw form
      QJsonObject walletJson = toJson();
      QByteArray walletRaw = QJsonDocument(walletJson).toJson();

      // Empty file
      _walletFile.resize(0);

      /*************************************
       ***** ENCRYPT HERE IN THE FUTURE ****
       *************************************/

      // Write to file
      _walletFile.write(walletRaw);

      _mutex.unlock();
  }

  quint32 Wallet::blockHeight() {

      // Talk to Chain.com or bitcore-explorer or something?

      return 0;
  }

  quint64 Wallet::balance(quint32 confirmations) {

      _mutex.lock();

      // Temporary balance
      quint64 balance = 0;

      // Get current block height
      quint32 currentBlockHeight = blockHeight();

      // Iterate entries and corresponding outputs
      for(QMap<PrivateKey, KeyInformation>::const_iterator i = _entries.constBegin();
          i != _entries.constEnd();i++) {

          // Iterate outputs
          const QMap<OutPoint, KeyInformation::Output> & outputs = i->outputs();
          for(QMap<OutPoint, KeyInformation::Output>::const_iterator i = outputs.constBegin();
              i != outputs.constEnd();i++) {

              // Check that output has sufficient number of confirmations
              if(currentBlockHeight - i->blockHeight() >= confirmations)
                  balance += i->value();
          }
      }

      _mutex.unlock();

      return balance;
  }

  void Wallet::synchronize() {

      // Only synchronized when nothing is locked, because this may disturb

      _mutex.lock();

      // For each private key, find all outputs, and update entry based on what
      // you found, that is add new outputs, and update values in old ones.

      _mutex.unlock();
  }
