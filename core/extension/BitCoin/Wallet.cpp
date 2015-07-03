/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "Wallet.hpp"
#include "BitSwaprjs.hpp"

#include <QJsonValue>
#include <QJsonObject>

#include "Utilities.hpp"

/**
 * Wallet::TxOEvent
 */

OldWallet::TxOEvent::TxOEvent() {
}

OldWallet::TxOEvent::TxOEvent(Type type,
                           const OutPoint & outpoint,
                           const quint64 value,
                           const QDateTime & firstSeen,
                           quint32 blockHeight)
    : _type(type)
    , _outpoint(outpoint)
    , _value(value)
    , _firstSeen(firstSeen)
    , _blockHeight(blockHeight) {
}

OldWallet::TxOEvent::TxOEvent(const QJsonObject & json) {

    // _type
    QString type = Utilities::GET_STRING(json, "type");

    if(type == "Send")
        _type = Type::Send;
    else if(type == "Receive")
        _type = Type::Receive;
    else
        throw new std::exception("type key must value among: {Send, Receive}");

    // _outpoint
    QJsonObject outpoint = Utilities::GET_OBJECT(json, "outpoint");
    _outpoint = OutPoint(outpoint);

    // _value
    _value = Utilities::GET_DOUBLE(json, "value");

    // _firstSeen
    QString firstSeen = Utilities::GET_STRING(json, "firstSeen");
    _firstSeen = QDateTime::fromString(firstSeen, Qt::ISODate);

    // _blockHeight
    _blockHeight = Utilities::GET_DOUBLE(json, "blockHeight");
}

bool OldWallet::TxOEvent::operator<(const TxOEvent & o) const {
    return _firstSeen < o.firstSeen();
}

QJsonObject OldWallet::TxOEvent::toJson() const {

    QJsonObject json;

    QString type;
    switch(_type) {

        case Type::Send:
            type = "Send";
            break;
        case Type::Receive:
            type = "Receive";
            break;
    }

    json["type"] = type;
    json["outpoint"] = QJsonValue(_outpoint.json());
    json["value"] = static_cast<double>(_value);
    json["firstSeen"] = _firstSeen.toString(Qt::ISODate);
    json["blockHeight"] = static_cast<double>(_blockHeight);

    return json;
}

OldWallet::TxOEvent::Type OldWallet::TxOEvent::type() const {
    return _type;
}

void OldWallet::TxOEvent::setType(Type type){
    _type = type;
}

OutPoint OldWallet::TxOEvent::outpoint() const {
    return _outpoint;
}

void OldWallet::TxOEvent::setOutpoint(const OutPoint & outpoint) {
    _outpoint = outpoint;
}

quint64 OldWallet::TxOEvent::value() const {
    return _value;
}

void OldWallet::TxOEvent::setValue(quint64 value) {
    _value = value;
}

QDateTime OldWallet::TxOEvent::firstSeen() const {
    return _firstSeen;
}

void OldWallet::TxOEvent::setFirstSeen(const QDateTime & firstSeen) {
    _firstSeen = firstSeen;
}

quint32 OldWallet::TxOEvent::blockHeight() const {
    return _blockHeight;
}

void OldWallet::TxOEvent::setBlockHeight(quint32 blockHeight) {
    _blockHeight = blockHeight;
}

/**
 * Wallet::Entry
 */

#include <QJsonArray>

  OldWallet::Entry::Entry() {

  }

  OldWallet::Entry::Entry(quint32 n,
                       const KeyPair & keyPair,
                       Purpose purpose,
                       const QDateTime & generated,
                       const QString & description,
                       const QMap<OutPoint, TxOEvent> & send,
                       const QMap<OutPoint, TxOEvent> & receive)
    : _n(n)
    , _keyPair(keyPair)
    , _purpose(purpose)
    , _generated(generated)
    , _description(description)
    , _send(send)
    , _receive(receive) {
  }

  OldWallet::Entry::Entry(const QJsonObject & json) {

      // _n
      _n = Utilities::GET_DOUBLE(json, "n");

      // _keyPair
      QJsonValue keyPair = json["keyPair"];
      if(keyPair.type() != QJsonValue::Object)
          throw new std::exception("keyPair key must map to QJsonValue::OJsonObject type.");

      _keyPair = KeyPair(keyPair.toObject());

      // _purpose
      QString purpose = Utilities::GET_STRING(json, "purpose");

      if(purpose == "Receive")
          _purpose = Purpose::Receive;
      else if(purpose == "ContractChange")
          _purpose = Purpose::ContractChange;
      else if(purpose == "BuyerInContractOutput")
          _purpose = Purpose::BuyerInContractOutput;
      else if(purpose == "ContractFinal")
          _purpose = Purpose::ContractFinal;
      else if(purpose == "SellerInContractOutput")
          _purpose = Purpose::SellerInContractOutput;
      else if(purpose == "ContractPayment")
          _purpose = Purpose::ContractPayment;
      else
          throw new std::exception("type key must have valud among {Receive, ContractChange, BuyerInContractOutput, ContractFinal, SellerInContractOutput, ContractPayment} of type QJsonValue::String.");

      // _generated
      QString generated = Utilities::GET_STRING(json, "generated");
      _generated = QDateTime::fromString(generated, Qt::ISODate);

      // _description
      _description = Utilities::GET_STRING(json, "description");

      // _send
      QJsonObject send = Utilities::GET_OBJECT(json, "send");

      for(QJsonObject::const_iterator i = send.constBegin();
          i != send.constEnd();i++) {

          // Key
          OutPoint key(i.key());

          // Value
          QJsonValue event = i.value();

          if(event.type() != QJsonValue::Object)
              throw new std::exception("send map must contain QJsonValue::QJsonObject values.");

          // Turn into object
          TxOEvent txOEvent(event.toObject());

          // Save in map
          _send[key] = txOEvent;
      }

      // _receive
      QJsonObject receive = Utilities::GET_OBJECT(json, "receive");

      for(QJsonObject::const_iterator i = receive.constBegin();
          i != receive.constEnd();i++) {

          // Key
          OutPoint key(i.key());

          // Value
          QJsonValue event = i.value();

          if(event.type() != QJsonValue::Object)
              throw new std::exception("receive map must contain QJsonValue::QJsonObject values.");

          // Turn into object
          TxOEvent txOEvent(event.toObject());

          // Save in map
          _receive[key] = txOEvent;
      }
      /**
      // _txoEvents
      QJsonArray txoEvents = Utilities::GET_ARRAY(json, "_txoEvents");

      for(QJsonArray::const_iterator i = txoEvents.constBegin();
          i != txoEvents.constEnd();i++) {

          // Get element
          QJsonValue txoEvent = *i;

          Q_ASSERT(txoEvent.type() == QJsonValue::Object);

          // Add to QVector
          _txoEvents.push_back(TxOEvent(txoEvent.toObject()));
      }
      */
  }

  void OldWallet::Entry::lockUtxo(const OutPoint & outPoint) {

      _locked.insert(outPoint);
  }

  bool OldWallet::Entry::isLocked(const OutPoint & outPoint) const {
      return _locked.contains(outPoint);
  }

  void OldWallet::Entry::unlockUtxo(const OutPoint & outPoint) {
      _locked.remove(outPoint);
  }

  QJsonObject OldWallet::Entry::toJson() const {

      QJsonObject json;

      json["n"] = static_cast<int>(_n);
      json["keyPair"] = _keyPair.json();

      // _purpose
      QString purpose;
      switch(_purpose) {

        case Purpose::Receive: purpose = "Receive"; break;
        case Purpose::ContractChange: purpose = "ContractChange"; break;
        case Purpose::BuyerInContractOutput: purpose = "BuyerInContractOutput"; break;
        case Purpose::ContractFinal: purpose = "ContractFinal"; break;
        case Purpose::SellerInContractOutput: purpose = "SellerInContractOutput"; break;
        case Purpose::ContractPayment: purpose = "ContractPayment"; break;
      }

      json["purpose"] = purpose;

      // _generated
      json["generated"] = _generated.toString(Qt::ISODate);

      // _description
      json["description"] = _description;

      // _send
      QJsonObject send;
      for(QMap<OutPoint, TxOEvent>::const_iterator i = _send.constBegin();
          i != _send.constEnd();i++)
          send[i.key().toString()] = i.value().toJson();

      json["send"] = send;

      // _receive
      QJsonObject receive;
      for(QMap<OutPoint, TxOEvent>::const_iterator i = _receive.constBegin();
          i != _receive.constEnd();i++)
          receive[i.key().toString()] = i.value().toJson();

      json["receive"] = receive;

      return json;
  }

  QList<OldWallet::TxOEvent> OldWallet::Entry::getFreeUtxo() const {

      QList<OldWallet::TxOEvent> utxos;

      for(QMap<OutPoint, TxOEvent>::const_iterator i = _receive.constBegin();
          i != _receive.constEnd();i++) {

          // Get event
          const TxOEvent & event = i.value();

          // Check that it is endeed an unspent output which is not locked
          if(event.type() == TxOEvent::Type::Receive && // receive event
                  !_send.contains(event.outpoint()) && // has not been spent
                  !_locked.contains(event.outpoint())) // is not locked
              utxos.append(event);
      }

      return utxos;
  }

  bool OldWallet::Entry::isUTxO(const OutPoint & outPoint) const {
      return _receive.contains(outPoint) && !_send.contains(outPoint);
  }

/**
  quint32 Wallet::Entry::updateUTXO() {

  }

  void Wallet::Entry::addTxOEvent(const TxOEvent & output) {

      OutPoint outpoint = output.point();

      // Check if outpoint already in map
      if(_outputs.contains(outpoint))
          throw new std::exception("Output already part of _outputs in KeyEntry.");

      // Store in map
      _outputs[outpoint] = output;
  }
*/

  quint32 OldWallet::Entry::n() const {
      return _n;
  }

  void OldWallet::Entry::setN(quint32 n) {
      _n = n;
  }

  KeyPair OldWallet::Entry::keyPair() const {
      return _keyPair;
  }

  void OldWallet::Entry::setKeyPair(const KeyPair & keyPair) {
      _keyPair = keyPair;
  }

  OldWallet::Purpose OldWallet::Entry::purpose() const {
      return _purpose;
  }

  void OldWallet::Entry::setPurpose(Purpose purpose) {
      _purpose = purpose;
  }

  QDateTime OldWallet::Entry::generated() const {
      return _generated;
  }

  void OldWallet::Entry::setGenerated(const QDateTime & added) {
      _generated = added;
  }

  QString OldWallet::Entry::description() const {
      return _description;
  }

  void OldWallet::Entry::setDescription(const QString & description) {
      _description = description;
  }

  QMap<OutPoint, OldWallet::TxOEvent> OldWallet::Entry::receive() const {
      return _receive;
  }

  void OldWallet::Entry::setReceive(const QMap<OutPoint, TxOEvent> &receive) {
      _receive = receive;
  }

  QMap<OutPoint, OldWallet::TxOEvent> OldWallet::Entry::send() const {
      return _send;
  }

  void OldWallet::Entry::setSend(const QMap<OutPoint, OldWallet::TxOEvent> &send) {
      _send = send;
  }

  /**
 * Wallet
 */

#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>

#include "UnspentP2PKHOutput.hpp"

  OldWallet::OldWallet(const QString & file, bool autoSave)
    : // add some of these hard coded values as explicit args in the future
      _walletSeed(0)
    , _gapLimit(1)
    , _keyCount(0)
    , _network(Network::testnet3)
    , _mutex(QMutex::Recursive) // allows same thread to call multiple synchronized sections in sequence
    , _walletFileName(file)
    , _autoSave(autoSave)
    , _walletFile(file)
    , _latestBlockHeight(0) {

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

      // Load from json
      fromJson(walletDictionary);

      // Compute wallet balance
      _lastComputedZeroConfBalance = computeBalance(0);
  }

  void OldWallet::fromJson(const QJsonObject & walletDictionary) {

      // If wallet was empty, we are done loading
      if(walletDictionary.size() == 0)
          return;

      // Parse out _walletSeed
      _walletSeed = Utilities::GET_DOUBLE(walletDictionary, "walletSeed");

      // Parse out _gabLimit
      _gapLimit = Utilities::GET_DOUBLE(walletDictionary, "gabLimit");

      // Parse out _keyCount
      _keyCount = Utilities::GET_DOUBLE(walletDictionary, "keyCount");

      // Parse out _entries
      QJsonObject entriesDictionary = Utilities::GET_OBJECT(walletDictionary, "entries");

      for(QJsonObject::const_iterator i = entriesDictionary.constBegin();
          i != entriesDictionary.constEnd();i++) {

          // Parse key into PrivateKey
          QJsonValue publicKeyValue = i.key();

          if(publicKeyValue.type() != QJsonValue::String)
              throw new std::exception("key in _entries not of type QJsonValue::String.");

          PublicKey key(publicKeyValue.toString());

          // Parse into KeyInformation
          QJsonValue keyInformationValue = i.value();

          if(keyInformationValue.type() != QJsonValue::Object)
            throw new std::exception("value in _entries not of type QJsonValue::Object.");

          // Save mapping
          _entries[key] = Entry(keyInformationValue.toObject());
      }

      // Parse out _chain
      QString chain = Utilities::GET_STRING(walletDictionary, "chain");

      if(chain == "mainnet")
          _network = Network::mainnet;
      else if(chain == "testnet3")
          _network = Network::testnet3;
      else
          throw new std::exception("chain key must have value among {mainnet, testnet3}.");

      // _latestBlockHeight
      _latestBlockHeight = Utilities::GET_DOUBLE(walletDictionary, "latestBlockHeight");
  }

  PrivateKey OldWallet::getSk(const PublicKey & pk) {

      _mutex.lock();

      // Check if we have this key in wallet
      if(!_entries.contains(pk)) {

          _mutex.unlock();

          throw new std::exception("No such public key in wallet.");

      }

      Entry & e = _entries[pk];

      _mutex.unlock();

      return e.keyPair().sk();
  }

  QJsonObject OldWallet::toJson() {

      QJsonObject wallet;

      _mutex.lock();

      // _walletSeed
      wallet["walletSeed"] = QJsonValue(_walletSeed);

      // _gabLimit
      wallet["gabLimit"] = QJsonValue(_gapLimit);

      // _keyCount
      wallet["keyCount"] = QJsonValue(_keyCount);

      // _entries
      QJsonObject entriesObject;

      for(QMap<PublicKey, Entry>::const_iterator i = _entries.constBegin();
          i != _entries.constEnd();i++)
        entriesObject[(i.key()).toString()] = (i.value()).toJson();

      wallet["entries"] = entriesObject;

      // _chain
      if(_network == Network::mainnet)
          wallet["chain"] = "mainnet";
      else
          wallet["chain"] = "testnet3";

      // _latestBlockHeight
      wallet["latestBlockHeight"] = QJsonValue(_latestBlockHeight);

      _mutex.unlock();

      return wallet;
  }

  int OldWallet::numberOfKeysInWallet() {

      int num;

      _mutex.lock();
      num = _entries.size();
      _mutex.unlock();

      return num;
  }

  void OldWallet::save() {

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

      // Flush to disk
      _walletFile.flush();

      _mutex.unlock();
  }

  quint64 OldWallet::computeBalance(quint32 confirmations) {

      _mutex.lock();

      // Temporary balance
      quint64 balance = 0;

      // Iterate entries and corresponding outputs
      for(QMap<PublicKey, Entry>::const_iterator i = _entries.constBegin();
          i != _entries.constEnd();i++) {

          // Iterate outputs
          const QMap<OutPoint, TxOEvent> & receive = i->receive();


          for(QMap<OutPoint, TxOEvent>::const_iterator j = receive.constBegin();
              j != receive.constEnd();j++) {

              // Check that output has sufficient number of confirmations
              if(_latestBlockHeight - j->blockHeight() >= confirmations && i->isUTxO(j.key()))
                  balance += j->value();
          }
      }

      _mutex.unlock();

      return balance;
  }

  #include <QDebug>

  void OldWallet::synchronize() {

      qDebug() << "Wallet::synchronize()";

      // Only synchronized when nothing is locked, because this may disturb

      _mutex.lock();

      // For each private key, find all outputs, and update entry based on what
      // you found, that is add new outputs, and update values in old ones.
      QList<PublicKey> & list = _entries.keys();

      if(list.size() > 0) {

          // Get events
          QMap<PublicKey, QList<OldWallet::TxOEvent>>  & keyEvents = BitSwaprjs::get_key_events(QSet<PublicKey>::fromList(list));

          for(QMap<PublicKey, QList<OldWallet::TxOEvent>>::const_iterator i = keyEvents.constBegin();
              i != keyEvents.constEnd();i++) {

              // Key
              const PublicKey & key = i.key();

              // Event list
              const QList<OldWallet::TxOEvent> & list = i.value();

              // Iterate list and crate new send and receiv maps
              QMap<OutPoint, TxOEvent> send, receive;

              for(QList<OldWallet::TxOEvent>::const_iterator j = list.constBegin();
                  j != list.constEnd();j++) {

                  const OldWallet::TxOEvent & event = *j;

                  if(event.type() == OldWallet::TxOEvent::Type::Receive) {
                      receive[event.outpoint()] = event;
                  } else {
                      send[event.outpoint()] = event;
                  }
              }

              // Update wallet entry
              Entry & entry = _entries[key];
              entry.setSend(send);
              entry.setReceive(receive);
          }

      }

      _latestBlockHeight = BitSwaprjs::get_latest_block();

      // Save wallet to disk if autosaving is enabled
      if(_autoSave)
          save();

      _mutex.unlock();

      quint64 balance = computeBalance(0);

      if(_lastComputedZeroConfBalance != balance) {

          _lastComputedZeroConfBalance = balance;
          emit zeroConfBalanceChanged(balance);
      }
  }

  QString OldWallet::toAddress(const PublicKey & pk) const {
      return BitSwaprjs::to_address(pk);
  }

  UnspentP2PKHOutput OldWallet::getUtxo(quint64 minimalValue, quint32 minimalNumberOfConfirmations) {

      /**
    return         UnspentP2PKHOutput(KeyPair(PublicKey("024f6f05e6f105fa0aaccfc36af9497d3929c4b33cbc8575445154e762f82eb08c"),
                                             PrivateKey("cNxShXWpyWug45tEieHgFCnfMCCMcmh6cKSQjh5peUDPJ5nZ49Hy")),
                                     OutPoint(TxId("99bd7714e8e4881c2766bea0ce6ca44ab9463417dd5040ae3664f41d7353df4c"),8),
                                     minimalValue);
*/
      _mutex.lock();

      // Go through entries
      for(QMap<PublicKey, Entry>::iterator i = _entries.begin();
          i != _entries.end();i++) {

          // Get entty
          Entry & entry = i.value();

          // Get free utxo in entry
          const QList<OldWallet::TxOEvent> & freeUtxo = entry.getFreeUtxo();

          // Find utxo which matches
          for(QList<OldWallet::TxOEvent>::const_iterator i = freeUtxo.constBegin();
              i != freeUtxo.constEnd();i++) {

              const OldWallet::TxOEvent & event = *i;

              // Check that utxo has enough value and confirmations, and is not locked
              if(event.value() >= minimalValue &&
                 _latestBlockHeight - event.blockHeight() >= minimalNumberOfConfirmations &&
                 !entry.isLocked(event.outpoint())) {

                  // Lock that utxo
                  entry.lockUtxo(event.outpoint());

                  // Return that utxo
                  UnspentP2PKHOutput utxo(entry.keyPair(), event.outpoint(), event.value());

                  _mutex.unlock();

                  return utxo;
              }
          }
      }

      _mutex.unlock();

      // Return 0 value utxo, which signifies failure.
      UnspentP2PKHOutput utxo;
      utxo.setValue(0);
      return utxo;
  }

#include <QDebug>

  OldWallet::Entry OldWallet::addReceiveKey(const QString & description, Purpose purpose) {

      _mutex.lock();

      // Generate key pair: future use _walletSeed and _keyCount to generate fresh key: PrivateKey(H(seed + n))
      QList<KeyPair> pairs = BitSwaprjs::generate_fresh_key_pairs(1);

      if(pairs.size() != 1)
          throw std::exception("Could not generate new key.");

      KeyPair & pair = pairs[0];

      // Create entry
      Entry entry = Entry(_keyCount++,
                            pair,
                            purpose,
                            QDateTime::currentDateTime(),
                            description,
                            QMap<OutPoint, TxOEvent>(),
                            QMap<OutPoint, TxOEvent>());

      // debug dump
      PublicKey pk = pair.pk();
      //qDebug() << "pk: " << pk.toString();

      // Add to map
      _entries[pk] = entry;

      //qDebug() << _entries.size();

      // Save wallet to disk if autosaving is enabled
      if(_autoSave)
          save();

      _mutex.unlock();

      // Return entry
      return entry;
  }

  QMap<PublicKey, OldWallet::Entry> OldWallet::generateNewKeys(quint8 numberOfKeys, Purpose purpose) {

      // Generate key pair: future use _walletSeed and _keyCount to generate fresh key: PrivateKey(H(seed + n))
      const QList<KeyPair> & pairs = BitSwaprjs::generate_fresh_key_pairs(numberOfKeys);

      _mutex.lock();

      QMap<PublicKey, OldWallet::Entry> newEntries;

      // Create new key
      for(quint8 i = 0;i < numberOfKeys;i++) {

          // Get key pair
          const KeyPair & pair = pairs[i];

          // Create entry
          Entry entry = Entry(_keyCount++,
                                    pair,
                                    purpose,
                                    QDateTime::currentDateTime(),
                                    "generateNewKeys()",
                                    QMap<OutPoint, TxOEvent>(),
                                    QMap<OutPoint, TxOEvent>());

          // Add to maps
          newEntries[pair.pk()] = entry;
          _entries[pair.pk()] = entry;
      }

      // Save wallet to disk if autosaving is enabled
      if(_autoSave)
          save();

      _mutex.unlock();

      return newEntries;
  }

  /*
  void Wallet::addEntry(const PublicKey & pk, const Entry & entry) {

      // Check if key already has entry
      if(_entries.contains(pk))
          throw new std::exception("key already has entry in wallet.");

      // Add to map
      _entries[pk] = entry;
  }


  void Wallet::addEntryOutput(const PublicKey & pk, const Entry::Output & output) {

      // Check if key is in map
      if(!_entries.contains(pk))
          throw new std::exception("key does not have entry in wallet.");

      // Get reference to entry
      Entry & entry = _entries[pk];

      // Add outpoint to entry
      entry.addTxOEvent(output);
  }
*/

  QMap<PublicKey, OldWallet::Entry> OldWallet::entries() {

      QMap<PublicKey, Entry> copy;

      _mutex.lock();
      copy = _entries;
      _mutex.unlock();

      return copy;
  }

  void OldWallet::setEntries(const QMap<PublicKey, Entry> &entries) {

      _mutex.lock();
      _entries = entries;
      _mutex.unlock();
  }

  OldWallet::Entry & OldWallet::entry(const PublicKey & publicKey) {
      return _entries[publicKey];
  }

  quint64 OldWallet::latestBlockHeight() {

      quint64 copy;

      _mutex.lock();
      copy = _latestBlockHeight;
      _mutex.unlock();

      return copy;
  }

  quint64 OldWallet::lastComputedZeroConfBalance() {

      quint64 copy;

      _mutex.lock();
      copy = _lastComputedZeroConfBalance;
      _mutex.unlock();

      return copy;
  }

  quint32 OldWallet::numberOfTransactions() {

      quint32 count = 0;

      _mutex.lock();

      // Iterate entries
      for(QMap<PublicKey, Entry>::const_iterator i = _entries.constBegin();
          i != _entries.constEnd();i++) {

          // Grab entry
          const Entry & entry = i.value();

          // Count sending transactions
          count += entry.send().size();

          // Count receiving transactions
          count += entry.receive().size();
      }

      _mutex.unlock();

      return count;
  }

/**
  void Wallet::setLatestBlockHeight(quint64 latestBlockHeight) {

      _mutex.lock();
      _latestBlockHeight = latestBlockHeight;
      _mutex.unlock();
  }
*/
