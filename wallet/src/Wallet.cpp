/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <common/Network.hpp>
#include <common/KeyPair.hpp>
#include <wallet/Wallet.hpp>
#include <wallet/WalletKey.hpp>
#include <wallet/WalletAddress.hpp>
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

Wallet::Wallet(const QString & walletFile)
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
    _nextHdIndex = WalletKey::numberOfKeysInWallet(_db);

    // Build utxo
    //updateUtxoSet();
}

void Wallet::createNewWallet(const QString & walletFile, Coin::Network network, const Seed & seed) {

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
    QSqlQuery query = WalletKey::createTableQuery(db);
    bool itWorked = query.exec();

    Q_ASSERT(itWorked);

    /**
    if(!WalletAddress::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create WalletAddress table.");
    }
    if(!BlockHeader::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create BlockHeader table.");
    }
    if(!Transaction::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create Transaction table.");
    }
    if(!OutPoint::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create OutPoint table.");
    }
    if(!Input::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create Input table.");
    }
    if(!TransactionHasInput::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create TransactionHasInput table.");
    }
    if(!Output::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create Output table.");
    }
    if(!TransactionHasOutput::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create TransactionHasOutput table.");
    }
    if(!InBoundPayment::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create InBoundPayment table.");
    }
    if(!OutBoundPayment::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create OutBoundPayment table.");
    }
    if(!Payer::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create Payer table.");
    }
    if(!OuputFundsPayer::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create OuputFundsPayer table.");
    }
    if(!Slot::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create Slot table.");
    }
    if(!Payee::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create Payee table.");
    }
    */

    // Close connection: Is this requried? I think destructor does this anyway.
    db.close();
}

bool Wallet::validateWalletStructure(QSqlDatabase & db) {
    return true;
}

Coin::Network Wallet::network() const {
    return _network;
}

QDateTime Wallet::created() const {
    return _created;
}

Seed Wallet::seed() const {
    return _seed;
}

quint64 Wallet::numberOfTransactions() {
    return 0;
}

quint64 Wallet::numberOfKeysInWallet() {
    return nextHdIndex();
}

quint64 Wallet::lastComputedZeroConfBalance() {

    quint64 balance;

    _mutex.lock();
    balance = _lastComputedZeroConfBalance;
    _mutex.unlock();

    return balance;
}

quint64 Wallet::nextHdIndex() {

    quint64 nextHdIndex;

    _mutex.lock();
    nextHdIndex = _nextHdIndex;
    _mutex.unlock();

    return nextHdIndex;
}

Coin::P2PKHAddress Wallet::getReceiveAddress() {

    // Generate fresh key
    Coin::PrivateKey sk = issueKey();

    // Get corresponding public key
    Coin::PublicKey pk = sk.toPublicKey();

    // Generate correspondig p2pkh address
    Coin::P2PKHAddress address(_network, pk);

    // Insert into database
    _mutex.lock();

    // Create wallet address object

    // get query

    // execute

    _mutex.unlock();

    return address;
}

Coin::PrivateKey Wallet::issueKey() {

    /**
     * UPDATE IN THE FUTURE TO WORK WITH KEY POOL
     */

    Coin::PrivateKey key;

    _mutex.lock();

    // Generate a new key by
    bytes_t rawPrivateKey = _keyChain.getPrivateSigningKey(_nextHdIndex);
    key = Coin::PrivateKey(rawPrivateKey);

    // Add to wallet key table
    QSqlQuery query = WalletKey(_nextHdIndex, key, QDateTime::currentDateTime(), true).insertQuery(_db);
    query.exec();
    QSqlError e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);

    // Add to key count
    _nextHdIndex++;

    _mutex.unlock();

    return key;
}

QList<Coin::KeyPair> Wallet::issueKeyPairs(quint64 numberOfPairs) {

    // List of keys to return
    QList<Coin::KeyPair> keys;

    // Create each pair and add to list
    for(quint64 i = 0;i < numberOfPairs;i++) {

        // Generate private key
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

void Wallet::updateUtxoSet() {

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

void Wallet::broadcast(const Coin::Transaction & tx) {

    _mutex.lock();

    // do something: may not need to be mutexed
    // save tx in wallet?

    _mutex.unlock();
}

/**
  UnspentP2PKHOutput Wallet::getUtxo(quint64 minimalValue, quint32 minimalNumberOfConfirmations) {

      return UnspentP2PKHOutput();
  }


QSqlQuery Wallet::createTransactionTableQuery() {

    return QSqlQuery("\
    CREATE TABLE Transaction (\
        id              BLOB        PRIMARY KEY,\
        version         INTEGER     NOT NULL,\
        lockTime        INTEGER     NOT NULL,\
        seen            INTEGER     NOT NULL,\
        blockNumber     INTEGER\
    )");
}

QSqlQuery Wallet::createInputTableQuery() {

    return QSqlQuery("\
    CREATE TABLE Input (\
        outPointTransactionId   BLOB,\
        outPointOutput          INTEGER,\
        PRIMARY KEY (outPointTransactionId, outPointOutput)\
        scriptSig               BLOB        NOT NULL,\
        sequence                INTEGER     NOT NULL,\
        scriptTypeId            INTEGER     NOT NULL FOREIGN KEY REFERENCES ScriptType(id)\
    )");
}

QSqlQuery Wallet::createScriptTypeTableQuery() {

    return QSqlQuery("\
    CREATE TABLE ScriptType (\
        id              INTEGER     PRIMARY KEY,\
        description     TEXT        NOT NULL\
    )");
}

QSqlQuery Wallet::createOutputTableQuery() {

    return QSqlQuery("\
    CREATE TABLE Output (\
        transactionId           BLOB        NOT NULL FOREIGN KEY REFERENCES Transaction(id),\
        output                  INTEGER,\
        PRIMARY KEY (transactionId, output),\
        value                   INTEGER     NOT NULL,\
        scriptPubKey            BLOB        NOT NULL,\
        sciptTypeId             INTEGER     NOT NULL FOREIGN KEY REFERENCES ScriptType(id),\
        receiveAddress          BLOB        FOREIGN KEY REFERENCES ReceiveAddress(address)\
    )");
}

QSqlQuery Wallet::createPayerTableQuery() {

    return QSqlQuery("\
    CREATE TABLE Payer (\
        fundingTransactionId   BLOB,\
        fundingOutput          INTEGER,\
        PRIMARY KEY (fundingTransactionId,fundingOutput),\
        created                INTEGER      NOT NULL,\
        description            TEXT\
    )");
}

QSqlQuery Wallet::createSlotTableQuery() {

    return QSqlQuery("\
    CREATE TABLE Slot (\
        payerFundingTransactionId   BLOB,\
        payerFundingOutput          INTEGER,\
        FOREIGN KEY (payerFundingTransactionId,payerFundingOutput) REFERENCES Output(transactionId, output),\
        index                       INTEGER,\
        PRIMARY KEY(payerFundingTransactionId, payerFundingOutput, index),\
        slotStateId                 INTEGER     NOT NULL FOREIGN KEY REFERENCES SlotState(id),\
        price                       INTEGER,\
        numberOfPaymentsMade        INTEGER     NOT NULL,\
        funds                       INTEGER,\
        lockTime                    INTEGER,\
        payerContractPrivateKeyId   INTEGER     NOT NULL FOREIGN KEY REFERENCES PrivateKey(id),\
        payerFinalPrivateKeyId      INTEGER     NOT NULL FOREIGN KEY REFERNCES PrivateKey(id),\
        payeeContractPublicKey      BLOB,\
        payeeFinalPublicKey         BLOB,\
        refundSignature             BLOB,\
        lastPaymentSignature        BLOB,\
        refundFee                   INTEGER,\
        paymentFee                  INTEGER\
    )");
}

QSqlQuery Wallet::createSlotStateTableQuery() {

    return QSqlQuery("\
    CREATE TABLE SlotState (\
        id              INTEGER     PRIMARY KEY,\
        description     TEXT        NOT NULL\
    )");
}

QSqlQuery Wallet::createPayeeTableQuery() {

    return QSqlQuery("\
    CREATE TABLE Payee (\
        id                              INTEGER     PRIMARY KEY,\
        payeeStateId                    INTEGER     NOT NULL FOREIGN KEY REFERENCES PayeeState(id),\
        numberOfPaymentsMade            INTEGER     NOT NULL,\
        lastValidPayerPaymentSignature  BLOB,\
        price                           INTEGER     NOT NULL,\
        funds                           INTEGER,\
        maximumNumberOfSellers          INTEGER     NOT NULL,\
        payeeContractPrivateKeyId       INTEGER     NOT NULL FOREIGN KEY REFERENCES PrivateKey(id),\
        payeePaymentPrivateKeyId        INTEGER     NOT NYLL FOREIGN KEY REFERENCES PrivateKey(id),\
        contractTransaction             BLOB,\
        contractOutput                  INTEGER,\
        payerContractPublicKey          BLOB,\
        payerFinalPublicKey             BLOB\
    )");
}

QSqlQuery Wallet::createPayeeStateTableQuery() {

    return QSqlQuery("\
    CREATE TABLE PayeeState (\
        id                              INTEGER     PRIMARY KEY,\
        text                            TEXT        NOT NULL\
    )");
}

QSqlQuery Wallet::insertReceiveAddressQuery() {

    return QSqlQuery("\
    INSERT INTO ReceiveAddress \
    (address, requested)\
    VALUES\
    (:address, :requested)\
    ");
}

QSqlQuery Wallet::insertReceiveAddressPurposeQuery() {

    return QSqlQuery("\
    INSERT INTO ReceiveAddressPurpose \
    (id, description)\
    VALUES\
    (:id, :description)\
    ");
}

QSqlQuery Wallet::insertPrivateKeyControllingReceiveAddressQuery() {

    return QSqlQuery("\
    INSERT INTO PrivateKeyControllingReceiveAddress \
    (address, privateKeyId, position)\
    VALUES\
    (:address, :privateKeyId, :position)\
    ");
}

QSqlQuery Wallet::insertTransactionQuery() {

    return QSqlQuery("\
    INSERT INTO Transaction \
    (id, version, lockTime, seen, blockNumber)\
    VALUES\
    (:id, :version, :lockTime, :seen, :blockNumber)\
    )");
}

QSqlQuery Wallet::insertInputQuery() {

    return QSqlQuery("\
    INSERT INTO Input \
    (outPointTransactionId, outPointOutput, scriptSig, sequence, scriptTypeId)\
    VALUES\
    (:outPointTransactionId, :outPointOutput, :scriptSig, :sequence, :scriptTypeId)\
    )");
}

QSqlQuery Wallet::insertScriptTypeQuery() {

    return QSqlQuery("\
    INSERT INTO ScriptType \
    (id, description)\
    VALUES\
    (:id, :description)\
    )");
}

QSqlQuery Wallet::insertOutputQuery() {

    return QSqlQuery("\
    INSERT INTO Output \
    (transactionId, output, value, scriptPubKey, sciptTypeId, receiveAddress)\
    VALUES\
    (:transactionId, :output, :value, :scriptPubKey, :sciptTypeId, :receiveAddress)\
    )");
}

QSqlQuery Wallet::insertPayerQuery() {

    return QSqlQuery("\
    INSERT INTO Payer \
    (fundingTransactionId, fundingOutput, created, description)\
    VALUES\
    (:fundingTransactionId, :fundingOutput, :created, :description)\
    )");
}

QSqlQuery Wallet::insertSlotQuery() {

    return QSqlQuery("\
    INSERT INTO Slot \
    (payerFundingTransactionId, payerFundingOutput, index, slotStateId, price, numberOfPaymentsMade, funds, lockTime, payerContractPrivateKeyId, payerFinalPrivateKeyId, payeeContractPublicKey, payeeFinalPublicKey, refundSignature, lastPaymentSignature, refundFee, paymentFee)\
    VALUES\
    (:payerFundingTransactionId, :payerFundingOutput, :index, :slotStateId, :price, :numberOfPaymentsMade, :funds, :lockTime, :payerContractPrivateKeyId, :payerFinalPrivateKeyId, :payeeContractPublicKey, :payeeFinalPublicKey, :refundSignature, :lastPaymentSignature, :refundFee, :paymentFee)\
    )");
}

QSqlQuery Wallet::insertSlotStateQuery() {

    return QSqlQuery("\
    INSERT INTO SlotState \
    (id, description)\
    VALUES\
    (:id, :description)\
    )");
}

QSqlQuery Wallet::insertPayeeQuery() {

    return QSqlQuery("\
    INSERT INTO Payee \
    (id, payeeStateId, numberOfPaymentsMade, lastValidPayerPaymentSignature, price, funds, maximumNumberOfSellers, payeeContractPrivateKeyId, payeePaymentPrivateKeyId, contractTransaction, contractOutput, payerContractPublicKey, payerFinalPublicKey)\
    VALUES\
    (:id, :payeeStateId, :numberOfPaymentsMade, :lastValidPayerPaymentSignature, :price, :funds, :maximumNumberOfSellers, :payeeContractPrivateKeyId, :payeePaymentPrivateKeyId, :contractTransaction, :contractOutput, :payerContractPublicKey, :payerFinalPublicKey)\
    )");
}

QSqlQuery Wallet::insertPayeeStateQuery() {

    return QSqlQuery("\
    INSERT INTO PayeeState \
    (id, text)\
    VALUES\
    (:id, :text)\
    )");
}

QSqlQuery Wallet::insertQuery(const ReceiveAddress & receiveAddress) {

    // Get templated key query
    QSqlQuery query = insertReceiveAddressQuery();

    return query;
}

QSqlQuery Wallet::insertQuery(const Payer & payer) {

    // Get templated key query
    QSqlQuery query = insertPayerQuery();

    return query;
}

QSqlQuery Wallet::insertQuery(const Payee & payee) {

    // Get templated key query
    QSqlQuery query = insertPayeeQuery();

    return query;
}

QSqlQuery Wallet::insertQuery(const Slot & slot) {

    // Get templated key query
    QSqlQuery query = insertSlotQuery();

    return query;
}
*/
