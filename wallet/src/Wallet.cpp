#include <wallet/Wallet.hpp>

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

QString Wallet::createPrivateKeyTableQuery() {

    return QStringLiteral("\
    CREATE TABLE PrivateKey (\
        id              INTEGER     PRIMARY KEY,\
        key             BLOB        NOT NULL,\
        keyPurposeId    INTEGER     NOT NULL FOREIGN KEY REFERENCES KeyPurpose(id),\
        generated       INTEGER     NOT NULL,\
        description     TEXT\
    )");
}

QString Wallet::createKeyPurposeTableQuery() {

    return QStringLiteral("\
    CREATE TABLE KeyPurpose (\
        id              INTEGER     PRIMARY KEY,\
        description     TEXT        NOT NULL\
    )");
}

QString Wallet::createReceiveAddressTableQuery() {

    return QStringLiteral("\
    CREATE TABLE ReceiveAddress (\
        address         BLOB        PRIMARY KEY,\
        requested       INTEGER     NOT NULL,\
    )");
}

QString Wallet::createPrivateKeyControllingReceiveAddressTableQuery() {

    return QStringLiteral("\
    CREATE TABLE PrivateKeyControllingReceiveAddress (\
        address         BLOB        FOREIGN KEY REFERENCES ReceiveAddress(address),\
        privateKeyId    INTEGER     FOREIGN KEY REFERENCES PrivateKey(id),\
        position        INTEGER     NOT NULL,\
        PRIMARY KEY(address, privateKeyId, position)\
    )");
}

QString Wallet::createTransactionTableQuery() {

    return QStringLiteral("\
    CREATE TABLE Transaction (\
        id              BLOB        PRIMARY KEY,\
        version         INTEGER     NOT NULL,\
        lockTime        INTEGER     NOT NULL,\
        seen            INTEGER     NOT NULL,\
        blockNumber     INTEGER\
    )");
}

QString Wallet::createInputTableQuery() {

    return QStringLiteral("\
    CREATE TABLE Input (\
        outPointTransactionId   BLOB,\
        outPointOutput          INTEGER,\
        PRIMARY KEY (outPointTransactionId, outPointOutput)\
        scriptSig               BLOB        NOT NULL,\
        sequence                INTEGER     NOT NULL,\
        scriptTypeId            INTEGER     NOT NULL FOREIGN KEY REFERENCES ScriptType(id)\
    )");
}

QString Wallet::createScriptTypeTableQuery() {

    return QStringLiteral("\
    CREATE TABLE ScriptType (\
        id              INTEGER     PRIMARY KEY,\
        description     TEXT        NOT NULL\
    )");
}

QString Wallet::createOutputTableQuery() {

    return QStringLiteral("\
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

QString Wallet::createPayerTableQuery() {

    return QStringLiteral("\
    CREATE TABLE Payer (\
        fundingTransactionId   BLOB,\
        fundingOutput          INTEGER,\
        PRIMARY KEY (fundingTransactionId,fundingOutput),\
        created                INTEGER      NOT NULL,\
        description            TEXT\
    )");
}

QString Wallet::createSlotTableQuery() {

    return QStringLiteral("\
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

QString Wallet::createSlotStateTableQuery() {

    return QStringLiteral("\
    CREATE TABLE SlotState (\
        id              INTEGER     PRIMARY KEY,\
        description     TEXT        NOT NULL\
    )");
}

QString Wallet::createPayeeTableQuery() {

    return QStringLiteral("\
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

QString Wallet::createPayeeStateTableQuery() {

    return QStringLiteral("\
    CREATE TABLE PayeeState (\
        id                              INTEGER     PRIMARY KEY,\
        text                            TEXT        NOT NULL\
    )");
}
