#include "Wallet.hpp"

/**
 * Wallet::Entry::Output
 */

 Wallet::Entry::Output::Output(const OutPoint & point,
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

 Wallet::Entry::Output::Output(const QJsonObject & json) {

     // _point
     QJsonValue pointValue = json["_point"];
     if(pointValue.type() != QJsonValue::Object)
         throw new std::exception("_point key does not exist or is not of type QJsonValue::Object.");

    QJsonObject pointObject = pointValue.toObject();

    _point = OutPoint(pointObject);

    // _added
    QJsonValue addedValue = json["_added"];
    if(addedValue.type() != QJsonValue::String)
        throw new std::exception("_added key does not exist or is not of type QJsonValue::String.");

    QString addedString = addedValue.toString();
    _added = QDateTime::fromString(addedString, Qt::ISODate);

    // _description
    QJsonValue descriptionValue = json["_description"];
    if(descriptionValue.type() != QJsonValue::String)
        throw new std::exception("_description key does not exist or is not of type QJsonValue::String.");

    _description = descriptionValue.toString();

    // _type
    QJsonValue typeString = json["_type"];
    if(typeString.type() != QJsonValue::String)
        throw new std::exception("_type key does not exist or is not of type QJsonValue::String.");

    QString type = typeString.toString();

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
    QJsonValue valueValue = json["_value"];

    if(valueValue.type() != QJsonValue::Double)
        throw new std::exception("_value key does not exist or is not of type QJsonValue::Double.");

    _value = valueValue.toDouble();

    // _blockHash
    QJsonValue blockHashString = json["_blockHash"];
    if(blockHashString.type() != QJsonValue::String)
        throw new std::exception("_blockHash key does not exist or is not of type QJsonValue::String.");

    _blockHash = Hash(blockHashString.toString());

    // _blockHeight
    QJsonValue blockHeightValue = json["_blockHeight"];

    if(blockHeightValue.type() != QJsonValue::Double)
        throw new std::exception("_blockHeight key does not exist or is not of type QJsonValue::Double.");

    _blockHeight = blockHeightValue.toDouble();

    // _spent
    QJsonValue spentValue = json["_spent"];
    if(spentValue.type() != QJsonValue::Bool)
        throw new std::exception("_spent key does not exist or is not of type QJsonValue::Bool.");

    _spent = QJsonValue(spentValue.toBool());
 }

 QJsonObject Wallet::Entry::Output::toJson() const {

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
     json["_value"] = QJsonValue(_value);
     json["_blockHash"] = QJsonValue(_blockHash.toString());
     json["_blockHeight"] = QJsonValue(_blockHeight);
     json["_spent"] = QJsonValue(_spent);

     return json;
 }

 OutPoint Wallet::Entry::Output::point() const {
     return _point;
 }

 void Wallet::Entry::Output::setPoint(const OutPoint & point) {
     _point = point;
 }

 QDateTime Wallet::Entry::Output::added() const {
     return _added;
 }

 void Wallet::Entry::Output::setAdded(const QDateTime & added) {
     _added = added;
 }

 QString Wallet::Entry::Output::description() const {
     return _description;
 }

 void Wallet::Entry::Output::setDescription(const QString & description) {
     _description = description;
 }

  Wallet::Entry::Output::Type Wallet::Entry::Output::type() const {
     return _type;
  }

  void Wallet::Entry::Output::setType(Type type) {
      _type = type;
  }

  Wallet::Entry::Output::State Wallet::Entry::Output::state() const {
      return _state;
  }

  void Wallet::Entry::Output::setState(State state) {
      _state = state;
  }

  quint64 Wallet::Entry::Output::value() const {
      return _value;
  }

  void Wallet::Entry::Output::setValue(quint64 value) {
      _value = value;
  }

  Hash Wallet::Entry::Output::blockHash() const {
      return _blockHash;
  }

  void Wallet::Entry::Output::setBlockHash(const Hash & blockHash) {
      _blockHash = blockHash;
  }

  quint32 Wallet::Entry::Output::blockHeight() const {
      return _blockHeight;
  }

  void Wallet::Entry::Output::setBlockHeight(quint32 blockHeight) {
      _blockHeight = blockHeight;
  }

  bool Wallet::Entry::Output::spent() const {
      return _spent;
  }

  void Wallet::Entry::Output::setSpent(bool spent) {
      _spent = spent;
  }

  /**
 * Wallet::Entry
 */

  Wallet::Entry::Entry(const QDateTime & added, const QString & description, const QMap<OutPoint, Output> & outputs)
    : _added(added)
    , _description(description)
    , _outputs(outputs) {
  }

  Wallet::Entry::Entry(const QJsonObject & json) {

      // Parse _added
      QJsonValue added = json["_added"];

      if(added.type() != QJsonValue::String)
          throw new std::exception("_added key is not present or not of type QJsonValue::String.");

      QString addedString = added.toString();

      _added = QDateTime::fromString(addedString, Qt::ISODate);

      // Parse _description
      QJsonValue description = json["_description"];

      if(description.type() != QJsonValue::String)
          throw new std::exception("_description key is not present or not of type QJsonValue::String.");

      _description = description.toString();

      // Parse _outputs
      QJsonValue outputsValue = json["_outputs"];

      if(outputsValue.type() != QJsonValue::Object)
          throw new std::exception("_outputs key is not present or not of type QJsonValue::Object.");

      QJsonObject ouputsObject = outputsValue.object();

      for(QJsonObject::const_iterator i = ouputsObject.constBegin();
          i != ouputsObject.constEnd();i++) {

          OutPoint key(i.key());

          // Parse Output
          QJsonValue valueValue = i.value();

          if(valueValue.type() != QJsonValue::Object)
              throw new std::exception("value in _outputs is not of type QJosnValue::Object.");

          QJsonObject valueObject(valueValue);

          // Save mapping
          _outputs[key] = Output(valueObject);
      }
  }

  QJsonObject Wallet::Entry::toJson() const {

      QJsonObject json;

      _mutex.lock();

      // _added
      json["_added"] = _added.toString(Qt::ISODate);

      // _description
      json["_description"] = QJsonValue(_description);

      // _outputs
      QJsonObject outputs;

      for(QMap<OutPoint, Output>::const_iterator i = _outputs.constBegin();
          i != _outputs.constEnd();i++)
          outputs[(i->key()).toString()] = Output(i->value());

      json["_outputs"] = outputs;

      _mutex.unlock();

      return json;
  }

  QDateTime Wallet::Entry::added() const {
      return _added;
  }

  void Wallet::Entry::setAdded(const QDateTime & added) {
      _added = added;
  }

  QString Wallet::Entry::description() const {
      return _description;
  }

  void Wallet::Entry::setDescription(const QString & description) {
      _description = description;
  }

  QMap<OutPoint, Wallet::Entry::Output> Entry::outputs() const {
      return _outputs;
  }

  void Wallet::Entry::setOutputs(const QMap<OutPoint, Wallet::Entry::Output> & outputs) {
      _outputs = outputs;
  }

/**
 * Wallet
 */

#include <QTextStream>

  Wallet::Wallet(const QString & file, bool autoSave)
    : _walletFileName(file)
    , _autoSave(autoSave)
    , _walletFile(file){

      // Open wallet file
      if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
          throw new std::exception("Could not open wallet file.");

      // Read entire file
      QByteArray walletRaw = file.readAll();

      /*************************************
       ***** DECRYPT HERE IN THE FUTURE ****
       *************************************/

      // Parse into json
      QJsonDocument doc(walletRaw);
      QJsonObject walletDictionary = doc.object();

      // Parse out _entries
      QJsonValue entries = walletDictionary["_entries"];

      if(entries.type() != QJsonValue::Object)
          throw new std::exception("_entries key not present or not of type QJsonValue::Object.");

      QJsonObject entriesDictionary = entries.toObject();

      for(QJsonObject::const_iterator i = entriesDictionary.constBegin();
          i != entriesDictionary.constEnd();i++) {

          // Parse key into PrivateKey
          QJsonValue privateKeyValue = i.key();

          if(privateKeyValue != QJsonValue::String)
              throw new std::exception("key in _entries not of type QJsonValue::String.");

          PrivateKey key(privateKeyValue.toString());

          // Parse into QList<Entry>
          QList<Entry> entryList;

          QJsonValue entryListValue = i.value();

          if(entryList != QJsonValue::Array)
              throw new std::exception("value in _entries not of type QJsonValue::Array.");

          QJsonArray entryListArray = entryListValue.toArray();

          for(QJsonArray::const_iterator i = entryListArray.constBegin();
              i != entryListArray.constEnd();i++) {

              // Parse Entry
              QJsonValue entryValue = *i;

              if(entryValue.type() != QJsonValue::Object)
                  throw new std::exception("element in value in _entries not of type QJsonValue::Object.");

              Entry entry(entryValue.toObject());

              // add to array
              entryList.push_back(entry);
          }

          // Save mapping
          _entries[key] = entryList;
      }

      // Parse out _entries
      QJsonValue chain = walletDictionary["_chain"];

      if(chain.type() != QJsonValue::String)
          throw new std::exception("_chain key not present or not of type QJsonValue::String.");
      else if(chain.toString() == "mainnet")
          _chain = Chain::mainnet;
      else if(chain.toString() == "testnet3")
          _chain = Chain::testnet3;
      else
          throw new std::exception("_chain key must have value among {mainnet, testnet3}.");
  }

  QJsonObject Wallet::toJson() const {

      QJsonObject wallet;

      _mutex.lock();

      // Jsonify _entries
      QJsonObject entriesObject;

      for(QMap<PrivateKey, QList<Entry>>::const_iterator i = _entries.constBegin();
          i != _entries.constEnd();i++) {

          QJsonArray entryListValue;

          QList<Entry> & entryList = i.value();
          for(QList<Entry>::const_iterator i = entryList.constBegin();
              i != entryList.constEnd();i++)
              entryListValue.push_back((*i).toJson());

          // Save
          entriesObject[(i->key).toString()] = entryListValue;
      }

      wallet["_entries"] = entriesObject;

      // Jsonify _chain
      if(_chain == Chain::mainnet)
          wallet["_chain"] = "mainnet";
      else
          wallet["_chain"] = "testnet3";

      _mutex.unlock();

      return wallet;
  }

  void Wallet::save() const {

      _mutex.lock();

      // Turn wallet into raw form
      QJsonObject walletJson = toJson();
      QByteArray walletRaw = QJsonDocument::toBinaryData(walletJson);

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
  }

  quint64 Wallet::balance(quint32 confirmations) const {

      _mutex.lock();

      // Temporary balance
      quint64 balance = 0;

      // Get current block height
      quint32 currentBlockHeight = blockHeight();

      // Iterate entries and corresponding outputs
      for(QMap<PrivateKey, QList<Entry>>::const_iterator i = _entries.constBegin();
          i != _entries.constEnd();i++) {

          // Grab entry list
          const QList<Entry> & entryList = i->second;

          // Itearte entry list
          for(QList<Entry>::const_iterator i = entryList.constBegin();
              i != entryList.constEnd();i++) {

              const QMap<OutPoint, Entry::Output> & outputs = i.outputs();

              // Iterate output map
              for(QMap<OutPoint, Entry::Output>::const_iterator i = outputs.constBegin();
                  i != outputs.constEnd();i++) {

                  Entry::Output & output = i->second();

                  // Check that output has sufficient number of confirmations
                  if(currentBlockHeight - output.blockHeight() >= confirmations)
                      balance += output.value();
              }
          }
      }

      _mutex.unlock();

      return balance;
  }

  void Wallet::synchronize() {

      _mutex.lock();

      _mutex.unlock();
  }
