/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <wallet/Wallet.hpp>
#include <wallet/WalletKey.hpp>

Wallet::Wallet(const QString & walletFile)
    : _walletFile(walletFile)
    , _db(QSqlDatabase::addDatabase("QSQLITE")) {

    // Try to open database
    _db.setDatabaseName(walletFile);

    if(!_db.open())
        throw std::exception("Could not open wallet file");

    if(!validateWalletStructure(_db))
        throw std::exception("Invalid wallet structure.");

    // Load metadata

    // Load key pools

    // Build utxo
}

Wallet::~Wallet() {

}

void Wallet::createEmptyWallet(const QString & walletFile, Network network, const QByteArray & seed) {

    // Metadata
}

bool Wallet::validateWalletStructure(QSqlDatabase & db) {
    return true;
}

QSqlQuery Wallet::createWalletKeyTableQuery() {

    return QSqlQuery("\
    CREATE TABLE WalletKey (\
        id              INTEGER     PRIMARY KEY,\
        key             BLOB        NOT NULL,\
        generated       INTEGER     NOT NULL\
    )");
}

QSqlQuery Wallet::createReceiveAddressTableQuery() {

    return QSqlQuery("\
    CREATE TABLE ReceiveAddress (\
        address         BLOB        PRIMARY KEY,\
        purpose         INTEGER     NOT NULL FOREIGN KEY REFERENCES ReceiveAddressPurpose(id),\
        requested       INTEGER     NOT NULL,\
        description     TEXT\
    )");
}

QSqlQuery Wallet::createReceiveAddressPurposeTableQuery() {

    return QSqlQuery("\
    CREATE TABLE ReceiveAddressPurpose (\
        id              INTEGER     PRIMARY KEY,\
        description     TEXT        NOT NULL\
    )");
}

QSqlQuery Wallet::createWalletKeyControllingReceiveAddressTableQuery() {

    return QSqlQuery("\
    CREATE TABLE WalletKeyControllingReceiveAddress (\
        address         BLOB        FOREIGN KEY REFERENCES ReceiveAddress(address),\
        walletKeyId     INTEGER     FOREIGN KEY REFERENCES WalletKey(id),\
        position        INTEGER     NOT NULL,\
        PRIMARY KEY(address, walletKeyId, position)\
    )");
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

QSqlQuery Wallet::insertWalletKeyQuery() {

    return QSqlQuery("\
    INSERT INTO PrivateKey \
    (id, key, keyPurposeId, generated, description)\
    VALUES\
    (:id, :key, :keyPurposeId, :generated, :description)\
    ");
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

/**
QSqlQuery Wallet::insertPrivateKeyControllingReceiveAddressQuery() {

    return QSqlQuery("\
    INSERT INTO PrivateKeyControllingReceiveAddress \
    (address, privateKeyId, position)\
    VALUES\
    (:address, :privateKeyId, :position)\
    ");
}
*/
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

QSqlQuery Wallet::insertQuery(const WalletKey & walletKey) {

    // Get templated key query
    QSqlQuery query = insertWalletKeyQuery();

    // bind wallet key values
    query.bindValue(":id", walletKey.walletSequenceNumber());
    //query.bindValue(":key", ...);
    query.bindValue(":keyPurposeId", WalletKey::encodePurpose(walletKey.purpose()));
    query.bindValue(":generated", walletKey.generated().toMSecsSinceEpoch());
    query.bindValue(":description", walletKey.description());

    return query;
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
