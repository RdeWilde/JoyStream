/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

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

Wallet::Wallet(const QString & walletFile)
    : _mutex(QMutex::Recursive) // allows same thread to call multiple synchronized sections in sequence
    , _walletFile(walletFile)
    , _db(QSqlDatabase::addDatabase("QSQLITE"))
    , _latestBlockHeight(0)
    , _lastComputedZeroConfBalance(0) {

    // If it doesnt exist, create an empty one, if it does exist,

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

    // Load key pools

    // Build utxo
}

void Wallet::createEmptyWallet(const QString & walletFile, Coin::Network network, const QByteArray & seed) {

    // Create connection to database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    Q_ASSERT(db.isValid());

    // Create metdata key-value store
    Metadata::createTable(db);

    // Populate with default values
    Metadata::populateTable(db, seed, network, QDateTime::currentDateTime());

    // Create tables
    if(!WalletKey::createTableQuery(db).exec()) {
        throw std::runtime_error("Could not create WalletKey table.");
    }
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
}

bool Wallet::validateWalletStructure(QSqlDatabase & db) {
    return true;
}

quint64 Wallet::lastComputedZeroConfBalance() {

    quint64 copy;

    _mutex.lock();
    copy = _lastComputedZeroConfBalance;
    _mutex.unlock();

    return copy;
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
