/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <common/Network.hpp>
#include <common/KeyPair.hpp>
#include <wallet/Manager.hpp>
#include <wallet/Key.hpp>
#include <wallet/Address.hpp>
#include <wallet/BlockHeader.hpp>
#include <wallet/Transaction.hpp>
#include <wallet/OutPoint.hpp>
#include <wallet/Input.hpp>
#include <wallet/TransactionHasInput.hpp>
#include <wallet/Output.hpp>
#include <wallet/TransactionHasOutput.hpp>
#include <wallet/InBoundPayment.hpp>
#include <wallet/OutBoundPayment.hpp>
#include <wallet/Payer.hpp>
#include <wallet/OuputFundsPayer.hpp>
#include <wallet/Slot.hpp>
#include <wallet/Payee.hpp>
#include <wallet/Metadata.hpp>

#include <CoinCore/typedefs.h> // bytes_t

namespace Wallet {

Manager::Manager(const QString & walletFile)
    : _mutex(QMutex::NonRecursive) // Same thread cannot aquire same lock multiple times
    , _walletFile(walletFile)
    , _db(QSqlDatabase::addDatabase(DATABASE_TYPE, QFileInfo(walletFile).fileName()))
    , _latestBlockHeight(0)
    , _lastComputedZeroConfBalance(0) {

    // If wallet does not exist, then create it
    if(!QFile(walletFile).exists()) {

        // Build error message
        QString errorMessage = "Was unable to read wallet file: " + walletFile;

        throw std::runtime_error(errorMessage.toStdString());
    }

    // Try to open database
    _db.setDatabaseName(walletFile);

    if(!_db.open())
        throw std::runtime_error("Could not open wallet file");

    if(!validateWalletStructure(_db))
        throw std::runtime_error("Invalid wallet structure.");

    // Load metadata
    _network = Metadata::getNetwork(_db);
    _seed = Metadata::getSeed(_db);
    _created = Metadata::getCreated(_db);

    // Set keychain based on seed
    _keyChain = _seed.generateHDKeychain();

    // Build key pool
    //updateKeyPool();

    // We count the number of keys in the wallet
    // so that the hd index counter can be set
    try {

        // Exception is thrown if wallet is empty, otherwise
        // the biggest index is returned.
        _nextIndex = Key::maxIndex(_db) + 1;

    } catch(std::runtime_error & e) {
        _nextIndex = 0;
    }

    // Build utxo
    //updateUtxoSet();
}

void Manager::createNewWallet(const QString & walletFile, Coin::Network network, const Seed & seed) {

    // Create connection to database:
    // We need non-default connection names, as testing will involve
    // running multiple clients which each start their own connections
    QString connectionName = QFileInfo(walletFile).fileName();

    // Create connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);

    Q_ASSERT(db.isValid());

    // Create database file
    db.setDatabaseName(walletFile);

    if(!db.open()) {

        // Build error message and throw exception
        QString errorMessage = "Could not open wallet file: " + walletFile;

        throw std::runtime_error(errorMessage.toStdString());
    }

    // Create metdata key-value store, and add rows for keys, and corresponding default values
    Metadata::createKeyValueStore(db, seed, network, QDateTime::currentDateTime());

    // Create relational tables
    // Working variables used in each query
    QSqlQuery query;
    QSqlError e;

    // Key
    query = Key::createTableQuery(db);
    query.exec();
    e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);

    // Address
    query = Address::createTableQuery(db);
    query.exec();
    e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);

    /**
      BlockHeader::createTableQuery(db).exec()

      Transaction::createTableQuery(db).exec()

      OutPoint::createTableQuery(db).exec()

      Input::createTableQuery(db).exec()

      TransactionHasInput::createTableQuery(db).exec()

      Output::createTableQuery(db).exec()

      TransactionHasOutput::createTableQuery(db).exec()

      InBoundPayment::createTableQuery(db).exec()

      OutBoundPayment::createTableQuery(db).exec()

      Payer::createTableQuery(db).exec()

      OuputFundsPayer::createTableQuery(db).exec()

      Slot::createTableQuery(db).exec()

      Payee::createTableQuery(db).exec()
    */

    // Close connection: Is this requried? I think destructor does this anyway.
    db.close();
}

bool Manager::validateWalletStructure(QSqlDatabase & db) {
    return true;
}

Coin::Network Manager::network() const {
    return _network;
}

QDateTime Manager::created() const {
    return _created;
}

Seed Manager::seed() const {
    return _seed;
}

quint64 Manager::numberOfTransactions() {
    return 0;
}

quint64 Manager::numberOfKeysInWallet() {
    return Key::numberOfKeysInWallet(_db);
}

quint64 Manager::lastComputedZeroConfBalance() {

    quint64 balance;

    _mutex.lock();
    balance = _lastComputedZeroConfBalance;
    _mutex.unlock();

    return balance;
}

/**
quint64 Wallet::nextHdIndex() {

    quint64 nextHdIndex;

    _mutex.lock();
    nextHdIndex = _nextIndex;
    _mutex.unlock();

    return nextHdIndex;
}
*/

Coin::PrivateKey Manager::issueKey() {
    return _issueKey().sk();
}

Coin::P2PKHAddress Manager::getReceiveAddress() {
    return _createReceiveAddress().address();
}

QList<Coin::KeyPair> Manager::issueKeyPairs(quint64 numberOfPairs) {

    // List of keys to return
    QList<Coin::KeyPair> keys;

    // Create each pair and add to list
    for(quint64 i = 0;i < numberOfPairs;i++) {

        // Get private key
        Coin::PrivateKey sk = issueKey();

        // Generate corresponding (compressd) public key
        Coin::PublicKey pk = sk.toPublicKey();

        // Add key pair to list
        keys.append(Coin::KeyPair(pk, sk));
    }

    return keys;
}

/**
void Wallet::releaseKeys(const QSet<Coin::KeyPair> & keys) {

    // for each key, if it is in dbase, but not in use, then place in key pool:

}

void Wallet::updateKeyPool() {

    // Net change to key pool
    quint32 diff = 0;

    _mutex.lock();

    // Scrap current pool
    _keyPool.clear();

    // Find all keys not currently used

    // Put in key pool

    _mutex.unlock();

    // Send signal of what changes were
    emit keyPoolUpdated(diff);
}
*/

void Manager::updateUtxoSet() {

    // Net change to txo
    quint32 diff = 0;

    _mutex.lock();

    // Scrap current utxo
    //_utxo.clear();

    // Find all utxo

    // Put in utxo

    _mutex.unlock();

    // Send signal of what changes were
    emit utxoUpdated(diff);
}

void Manager::broadcast(const Coin::Transaction & tx) {

    _mutex.lock();

    // do something: may not need to be mutexed
    // save tx in wallet?

    _mutex.unlock();
}

Key::Record Manager::_issueKey() {

    /**
     * ==========================================
     * UPDATE IN THE FUTURE TO WORK WITH KEY POOL
     * ==========================================
     */

    Coin::PrivateKey key;

    // We aquire lock so we protect our value of _nextIndex
    _mutex.lock();

    // Generate a new key by
    bytes_t rawPrivateKey = _keyChain.getPrivateSigningKey(_nextIndex);
    key = Coin::PrivateKey(rawPrivateKey);

    // Create wallet key entry
    Key::Record record(_nextIndex, key, QDateTime::currentDateTime(), true);

    // Add to table
    QSqlQuery query = record.insertQuery(_db);
    query.exec();
    QSqlError e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);

    // Add to key count
    Q_ASSERT(_nextIndex == Key::maxIndex(_db));

    // Increment to next index
    _nextIndex++;

    _mutex.unlock();

    return record;
}

Address::Record Manager::_createReceiveAddress() {

    // Generate fresh wallet key
    Key::Record keyRecord = _issueKey();

    // Get private key
    Coin::PrivateKey sk = keyRecord.sk();

    // Get corresponding public key
    Coin::PublicKey pk =  sk.toPublicKey();

    // Create wallet address
    Address::Record addressRecord(keyRecord.index(), Coin::P2PKHAddress(_network, pk));

    // Insert into wallet
    QSqlQuery query = addressRecord.insertQuery(_db);
    query.exec();
    QSqlError e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);

    return addressRecord;
}

}
