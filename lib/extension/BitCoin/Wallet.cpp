#include "Wallet.hpp"
#include "BitSwaprjs.hpp"

#include <QJsonValue>
#include <QJsonObject>

#include "Utilities.hpp"

/**
 * Wallet::Entry::Output
 */

 Wallet::KeyEntry::Output::Output() {
 }

 Wallet::KeyEntry::Output::Output(const OutPoint & point,
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

 Wallet::KeyEntry::Output::Output(const QJsonObject & json) {

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

 bool Wallet::KeyEntry::Output::operator<(const Output & rhs) {
     return _point < rhs.point();
 }

 QJsonObject Wallet::KeyEntry::Output::toJson() const {

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

 OutPoint Wallet::KeyEntry::Output::point() const {
     return _point;
 }

 void Wallet::KeyEntry::Output::setPoint(const OutPoint & point) {
     _point = point;
 }

 QDateTime Wallet::KeyEntry::Output::added() const {
     return _added;
 }

 void Wallet::KeyEntry::Output::setAdded(const QDateTime & added) {
     _added = added;
 }

 QString Wallet::KeyEntry::Output::description() const {
     return _description;
 }

 void Wallet::KeyEntry::Output::setDescription(const QString & description) {
     _description = description;
 }

  Wallet::KeyEntry::Output::Type Wallet::KeyEntry::Output::type() const {
     return _type;
  }

  void Wallet::KeyEntry::Output::setType(Type type) {
      _type = type;
  }

  Wallet::KeyEntry::Output::State Wallet::KeyEntry::Output::state() const {
      return _state;
  }

  void Wallet::KeyEntry::Output::setState(State state) {
      _state = state;
  }

  quint64 Wallet::KeyEntry::Output::value() const {
      return _value;
  }

  void Wallet::KeyEntry::Output::setValue(quint64 value) {
      _value = value;
  }

  Hash Wallet::KeyEntry::Output::blockHash() const {
      return _blockHash;
  }

  void Wallet::KeyEntry::Output::setBlockHash(const Hash & blockHash) {
      _blockHash = blockHash;
  }

  quint32 Wallet::KeyEntry::Output::blockHeight() const {
      return _blockHeight;
  }

  void Wallet::KeyEntry::Output::setBlockHeight(quint32 blockHeight) {
      _blockHeight = blockHeight;
  }

  bool Wallet::KeyEntry::Output::spent() const {
      return _spent;
  }

  void Wallet::KeyEntry::Output::setSpent(bool spent) {
      _spent = spent;
  }

  /**
 * Wallet::Entry
 */

  Wallet::KeyEntry::KeyEntry() {

  }

  Wallet::KeyEntry::KeyEntry(quint32 n,
                             const KeyPair & keyPair,
                             Source source,
                             const QDateTime & added,
                             const QString & description,
                             const QMap<OutPoint, Output> & outputs)
    : _n(n)
    , _keyPair(keyPair)
    , _source(source)
    , _added(added)
    , _description(description)
    , _outputs(outputs) {
  }

  Wallet::KeyEntry::KeyEntry(const QJsonObject & json) {

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

  QJsonObject Wallet::KeyEntry::toJson() const {

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

  Wallet::KeyEntry::Source Wallet::KeyEntry::source() const {
      return _source;
  }

  void Wallet::KeyEntry::setSource(Source source) {
      _source = source;
  }

  KeyPair Wallet::KeyEntry::keyPair() const {
      return _keyPair;
  }

  void Wallet::KeyEntry::setKeyPair(const KeyPair & keyPair) {
      _keyPair = keyPair;
  }

  quint32 Wallet::KeyEntry::n() const {
      return _n;
  }

  void Wallet::KeyEntry::setN(quint32 n) {
      _n = n;
  }

  QDateTime Wallet::KeyEntry::added() const {
      return _added;
  }

  void Wallet::KeyEntry::setAdded(const QDateTime & added) {
      _added = added;
  }

  QString Wallet::KeyEntry::description() const {
      return _description;
  }

  void Wallet::KeyEntry::setDescription(const QString & description) {
      _description = description;
  }

  QMap<OutPoint, Wallet::KeyEntry::Output> Wallet::KeyEntry::outputs() const {
      return _outputs;
  }

  void Wallet::KeyEntry::setOutputs(const QMap<OutPoint, Wallet::KeyEntry::Output> & outputs) {
      _outputs = outputs;
  }

/**
  bool Wallet::KeyEntry::containsOutPoint(const OutPoint & p) {
    return _outputs.contains(p);
  }
*/

  void Wallet::KeyEntry::addOutPoint(const Output & output) {

      OutPoint outpoint = output.point();

      // Check if outpoint already in map
      if(_outputs.contains(outpoint))
          throw new std::exception("Output already part of _outputs in KeyEntry.");

      // Store in map
      _outputs[outpoint] = output;
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
    , _walletFile(file) {

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

      // If wallet was empty, we are done loading
      if(walletDictionary.size() == 0)
          return;

      // Parse out _walletSeed
      _walletSeed = Utilities::GET_DOUBLE(walletDictionary, "_walletSeed");

      // Parse out _gabLimit
      _gabLimit = Utilities::GET_DOUBLE(walletDictionary, "_gabLimit");

      // Parse out _keyCount
      _keyCount = Utilities::GET_DOUBLE(walletDictionary, "_keyCount");

      // Parse out _entries
      QJsonObject entriesDictionary = Utilities::GET_OBJECT(walletDictionary, "_entries");

      for(QJsonObject::const_iterator i = entriesDictionary.constBegin();
          i != entriesDictionary.constEnd();i++) {

          // Parse key into PrivateKey
          QJsonValue publicKeyValue = i.key();

          if(publicKeyValue != QJsonValue::String)
              throw new std::exception("key in _entries not of type QJsonValue::String.");

          PublicKey key(publicKeyValue.toString());

          // Parse into KeyInformation
          QJsonValue keyInformationValue = i.value();

          if(keyInformationValue.type() != QJsonValue::Object)
            throw new std::exception("value in _entries not of type QJsonValue::Object.");

          // Save mapping
          _entries[key] = KeyEntry(keyInformationValue.toObject());
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

      for(QMap<PublicKey, KeyEntry>::const_iterator i = _entries.constBegin();
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

  int Wallet::numberOfKeysInWallet() {

      int num;

      _mutex.lock();
      num = _entries.size();
      _mutex.unlock();

      return num;
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
      for(QMap<PublicKey, KeyEntry>::const_iterator i = _entries.constBegin();
          i != _entries.constEnd();i++) {

          // Iterate outputs
          const QMap<OutPoint, KeyEntry::Output> & outputs = i->outputs();
          for(QMap<OutPoint, KeyEntry::Output>::const_iterator i = outputs.constBegin();
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

  QString Wallet::toAddress(const PublicKey & pk) const {
      return BitSwaprjs::to_address(pk);
  }

  Wallet::KeyEntry Wallet::addReceiveKey(const QString & description) {

      _mutex.lock();

      // Generate key pair: future use _walletSeed and _keyCount to generate fresh key: PrivateKey(H(seed + n))
      QList<KeyPair> pairs = BitSwaprjs::generate_fresh_key_pairs(1);

      if(pairs.size() != 1)
          throw std::exception("Could not generate new key.");

      KeyPair & pair = pairs[0];

      // Create entry
      KeyEntry entry = KeyEntry(_keyCount++,
                                pair,
                                KeyEntry::Source::Generated,
                                QDateTime::currentDateTime(),
                                description,
                                QMap<OutPoint, KeyEntry::Output>());
      // Add to map
      _entries[pair.pk()] = entry;

      _mutex.unlock();

      // Return entry
      return entry;
  }

  QMap<PublicKey, Wallet::KeyEntry> Wallet::generateNewKeys(quint8 numberOfKeys) {

      // Generate key pair: future use _walletSeed and _keyCount to generate fresh key: PrivateKey(H(seed + n))
      const QList<KeyPair> & pairs = BitSwaprjs::generate_fresh_key_pairs(numberOfKeys);

      _mutex.lock();

      QMap<PublicKey, Wallet::KeyEntry> newEntries;

      // Create new key
      for(quint8 i = 0;i < numberOfKeys;i++) {

          // Get key pair
          const KeyPair & pair = pairs[i];

          // Create entry
          KeyEntry entry = KeyEntry(_keyCount++,
                                    pair,
                                    KeyEntry::Source::Generated,
                                    QDateTime::currentDateTime(),
                                    "generateNewKeys()",
                                    QMap<OutPoint, KeyEntry::Output>());

          // Add to maps
          newEntries[pair.pk()] = entry;
          _entries[pair.pk()] = entry;
      }

      _mutex.unlock();

      return newEntries;
  }

  void Wallet::addEntry(const PublicKey & pk, const KeyEntry & entry) {

      // Check if key already has entry
      if(_entries.contains(pk))
          throw new std::exception("key already has entry in wallet.");

      // Add to map
      _entries[pk] = entry;
  }

  void Wallet::addEntryOutput(const PublicKey & pk, const KeyEntry::Output & output) {

      // Check if key is in map
      if(!_entries.contains(pk))
          throw new std::exception("key does not have entry in wallet.");

      // Get reference to entry
      KeyEntry & entry = _entries[pk];

      // Add outpoint to entry
      entry.addOutPoint(output);
  }

  QMap<PublicKey, Wallet::KeyEntry> Wallet::entries() {

      QMap<PublicKey, KeyEntry> copy;

      _mutex.lock();
      copy = _entries;
      _mutex.unlock();

      return copy;
  }

  void Wallet::setEntries(const QMap<PublicKey, KeyEntry> &entries) {

      _mutex.lock();
      _entries = entries;
      _mutex.unlock();
  }

  quint64 Wallet::latestBlockHeight() {

      quint64 copy;

      _mutex.lock();
      copy = _latestBlockHeight;
      _mutex.unlock();

      return copy;
  }
/**
  void Wallet::setLatestBlockHeight(quint64 latestBlockHeight) {

      _mutex.lock();
      _latestBlockHeight = latestBlockHeight;
      _mutex.unlock();
  }
*/
