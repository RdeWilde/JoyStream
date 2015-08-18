/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

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
#include <wallet/UtxoCreated.hpp>
#include <wallet/UtxoDestroyed.hpp>

#include <common/Network.hpp>
#include <common/KeyPair.hpp>
#include <common/Utilities.hpp>

#include <CoinCore/typedefs.h> // bytes_t
#include <CoinCore/CoinNodeData.h> // Coin::CoinBlockHeader, Transaction types: outpoints, transactions, ...
#include <CoinQ/CoinQ_script.h> // getAddressForTxOutScript

#include <QString>

namespace Wallet {

Manager::Manager(const QString & walletFile)
    : _mutex(QMutex::Recursive) // Same thread CAN aquire same lock multiple times, but must do parity unlocks
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

void Manager::createNewWallet(const QString & walletFile, Coin::Network network, const Coin::Seed & seed) {

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
    bool ok;

    // Key
    ok = Key::createTable(db);
    Q_ASSERT(ok);

    // Address
    ok = Address::createTable(db);

    // BlockHeader
    BlockHeader::createTable(db);
    OutPoint::createTable(db);
    Input::createTable(db);
    Output::createTable(db);
    Transaction::createTable(db);
    TransactionHasInput::createTable(db);
    TransactionHasOutput::createTable(db);

    /**

      ===============================
      InBoundPayment::createTableQuery(db).exec()

      OutBoundPayment::createTableQuery(db).exec()

      ===============================
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

Coin::Seed Manager::seed() const {
    return _seed;
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
    return _issueKey()._sk;
}

Coin::P2PKHAddress Manager::getReceiveAddress() {
    return _createReceiveAddress()._address;
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

quint64 Manager::numberOfKeysInWallet() {
    return Key::numberOfKeysInWallet(_db);
}

bool Manager::addBlockHeader(const Coin::CoinBlockHeader & blockHeader, quint64 numberOfTransactions, bool isOnMainChain, quint32 totalProofOfWork) {

    // Create private key to check if record exists
    BlockHeader::PK pk(blockHeader.getHashLittleEndian());

    // Lock so that checking existence and insertion are atomic
    _mutex.lock();

    // We are done if it already exists
    if(BlockHeader::exists(_db, pk)) {

        _mutex.unlock();
        return false;
    }

    // Create record
    BlockHeader::Record record(blockHeader, numberOfTransactions, isOnMainChain, totalProofOfWork);

    // Insert record
    bool ok = BlockHeader::insert(_db, record);
    Q_ASSERT(ok);

    // Signal event
    emit blockHeaderAdded(blockHeader); // drop the extra stuff!!

    _mutex.unlock();
    return true;
}

bool Manager::addOutPoint(const Coin::OutPoint & outPoint) {

    // Create private key
    OutPoint::PK pk(outPoint.getHashLittleEndian(), outPoint.index);

    // Lock so that checking existence and insertion are atomic
    _mutex.lock();

    // We are done if it already exists
    if(OutPoint::exists(_db, pk)) {

        _mutex.unlock();
        return false;
    }

    // Create record
    OutPoint::Record record(pk);

    // Insert record
    bool ok = OutPoint::insert(_db, record);
    Q_ASSERT(ok);

    // Signal event
    emit outPointAdded(outPoint);

    _mutex.unlock();
    return true;
}

bool Manager::addInput(const Coin::TxIn & txIn) {

    // Lets blindly attempt to add outpoint, it may or may not work
    addOutPoint(txIn.previousOut);

    // Create private key
    const char * raw = (const char *)txIn.scriptSig.data();
    Input::PK pk(txIn.previousOut, QByteArray::fromRawData(raw,txIn.scriptSig.size()), txIn.sequence);

    // Lock so that checking existence and insertion are atomic
    _mutex.lock();

    // We are done if it already exists
    if(Input::exists(_db, pk)) {

        _mutex.unlock();
        return false;
    }

    // Create record
    Input::Record record(pk);

    // Insert it
    bool ok = Input::insert(_db, record);
    Q_ASSERT(ok);

    // Signal event
    emit inputAdded(txIn);

    _mutex.unlock();
    return true;
}

bool Manager::addOutput(const Coin::TxOut & txOut) {

    // Lock so that checking existence and insertion are atomic
    _mutex.lock();

    // Get corresponding record for address corresponding to output script, if it exists
    Address::Record record;
    bool exists = Manager::getAddressForOutput(txOut, record);

    // Create primary key for output
    Output::PK pk(txOut.value, Coin::toByteArray(txOut.scriptPubKey));

    if(Output::exists(_db, pk)) {

        Q_ASSERT(exists); // output exists => outpoint exists
        _mutex.unlock();
        return false;
    }

    // Create output record
    Output::Record outputRecord(pk, exists ? QVariant(record._keyIndex) : QVariant());

    // Insert it
    bool ok = Output::insert(_db, outputRecord);
    Q_ASSERT(ok);

    // Signal event
    emit outputAdded(txOut);

    _mutex.unlock();
    return true;
}

bool Manager::addTransaction(const Coin::Transaction & transaction) {

    // Lock so that checking existence and insertion are atomic
    _mutex.lock();

    /**
    if(!Transaction::exists(Record::PK())) {

        // inputs
        // addInput(const Coin::TxIn & txIn)

        // outputs
        // addOutput(const Coin::TxOut & txOut)

        // Tx

        // Hasinput
        // TransactionHasOutput : add hidden routines

        // Has output
        // TransactionHasInput: add hidden routine

        emit transactionAdded(transaction);

        _mutex.unlock();
        return true;

    } else {
        _mutex.unlock();
        return false;
    }

    */

    return false;
}

Coin::Transaction Manager::getTransaction(const Coin::TransactionId & transactionId) {
    throw std::runtime_error("not implemented");
}

QList<Coin::Transaction> Manager::allTransactions() {
    throw std::runtime_error("not implemented");
}

quint64 Manager::numberOfTransactions() {
    throw std::runtime_error("not implemented");
}

QList<UtxoCreated> getAllUtxoCreated() {
    throw std::runtime_error("not implemented");
}

QList<UtxoDestroyed> getAllUtxoDestroyed() {
    throw std::runtime_error("not implemented");
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

Address::Record Manager::_createReceiveAddress() {

    // Generate fresh wallet key
    Key::Record keyRecord = _issueKey();

    // Get private key
    Coin::PrivateKey sk = keyRecord._sk;

    // Get corresponding public key
    Coin::PublicKey pk = sk.toPublicKey();

    // Create wallet address
    Address::Record addressRecord(keyRecord._index, Coin::P2PKHAddress(_network, pk));

    // Insert into wallet
    Address::insert(_db, addressRecord);

    return addressRecord;
}


Key::Record Manager::_issueKey() {

    /**
     * ==========================================
     * UPDATE IN THE FUTURE TO WORK WITH KEY POOL
     * ==========================================
     */

    // We aquire lock so we protect our value of _nextIndex
    _mutex.lock();

    // Generate a new key by
    bytes_t rawPrivateKey = _keyChain.getPrivateSigningKey(_nextIndex);
    Coin::PrivateKey key = Coin::PrivateKey(rawPrivateKey);

    // Create wallet key entry
    Key::Record record(_nextIndex, key, QDateTime::currentDateTime(), true);

    // Add to table
    Key::insert(_db, record);
    Q_ASSERT(_nextIndex == Key::maxIndex(_db));

    // Increment to next index
    _nextIndex++;

    _mutex.unlock();

    return record;
}

bool Manager::getAddressForOutput(const Coin::TxOut & txOut, Address::Record & record) {

    // Deduce address from output script
    QString base58CheckEncodedString = QString::fromStdString(CoinQ::Script::getAddressForTxOutScript(txOut.scriptPubKey, Coin::networkToAddressVersions(_network)));

    // Get address from base58checkencoded string
    Coin::P2PKHAddress address = Coin::P2PKHAddress::fromBase58CheckEncoding(base58CheckEncodedString);
    Q_ASSERT(address.network() == _network);

    return Address::findFromAddress(_db, address, record);
}

}
