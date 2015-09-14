/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/BlockHeader.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::CoinBlockHeader

#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace BlockHeader {

Record::Record(){
}

Record::Record(PK blockId,
                quint32 version,
                const Coin::BlockId & previousBlockId,
                const Coin::TransactionMerkleTreeRoot & merkleRoot,
                const QDateTime & timeStamp,
                quint32 nBits,
                quint32 nonce,
                quint64 numberOfTransactions,
                bool isOnMainChain,
                quint32 totalProofOfWork,
                quint64 blockHeight)
    : _blockId(blockId)
    , _version(version)
    , _previousBlockId(previousBlockId)
    , _merkleRoot(merkleRoot)
    , _timeStamp(timeStamp)
    , _nBits(nBits)
    , _nonce(nonce)
    , _transactionCount(numberOfTransactions)
    , _isOnMainChain(isOnMainChain)
    , _totalProofOfWork(totalProofOfWork)
    , _blockHeight(blockHeight){
}

Record::Record(const Coin::CoinBlockHeader & h,
               quint64 numberOfTransactions,
               bool isOnMainChain,
               quint32 totalProofOfWork,
               quint64 blockHeight)
    : Record(h.getHash(),
             h.version(),
             h.prevBlockHash(),
             h.merkleRoot(),
             QDateTime::fromMSecsSinceEpoch(h.timestamp()),
             h.bits(),
             h.nonce(),
             numberOfTransactions,
             isOnMainChain,
             totalProofOfWork,
             blockHeight) {
}

Record::Record(const QSqlRecord & record){

    _blockId = record.value("blockId").toByteArray();
    _version = record.value("version").toUInt();
    _previousBlockId = record.value("previousBlockId").toByteArray();
    _merkleRoot = record.value("merkleRoot").toByteArray();
    _timeStamp = record.value("timeStamp").toDateTime();
    _nBits = record.value("version").toUInt();
    _nonce = record.value("nonce").toUInt();

    bool ok;
    _transactionCount = record.value("transactionCount").toULongLong(&ok);
    Q_ASSERT(ok);

    _isOnMainChain = record.value("isOnMainChain").toBool();
    _totalProofOfWork = record.value("version").toUInt();
}

bool createTable(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE BlockHeader ( "
        "blockId             BLOB        NOT NULL, "
        "version             INTEGER     NOT NULL, "
        "previousBlockId     BLOB        NOT NULL, "
        "merkleRoot          BLOB        NOT NULL, "
        "timeStamp           INTEGER     NOT NULL, "
        "bits                INTEGER     NOT NULL, "
        "nonce               INTEGER     NOT NULL, "
        "transactionCount    INTEGER     NOT NULL, "
        "isOnMainChain       INTEGER     NOT NULL, "
        "totalProofOfWork    INTEGER     NOT NULL, "
        "blockHeight         INTEGER     NOT NULL, "
        "PRIMARY KEY(blockId), "
        "UNIQUE(version, previousBlockId, merkleRoot, timeStamp, bits, nonce) "
    ")");

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool insert(QSqlDatabase db, const Record & record) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO BlockHeader "
        "(blockId, version, previousBlockId, merkleRoot, timeStamp, bits, nonce, transactionCount, isOnMainChain, totalProofOfWork) "
    "VALUES "
        "(:blockId, :version, :previousBlockId, :merkleRoot, :timeStamp, :bits, :nonce, :transactionCount, :isOnMainChain, :totalProofOfWork) "
    );

    // Bind values to query fields
    query.bindValue(":blockId", record._blockId.toByteArray());
    query.bindValue(":version", record._version);
    query.bindValue(":previousBlockId", record._previousBlockId.toByteArray());
    query.bindValue(":merkleRoot", record._merkleRoot.toByteArray());
    query.bindValue(":timeStamp", record._timeStamp.toMSecsSinceEpoch());
    query.bindValue(":bits", record._nBits);
    query.bindValue(":nonce", record._nonce);
    query.bindValue(":transactionCount", record._transactionCount);
    query.bindValue(":isOnMainChain", record._isOnMainChain ? 1 : 0);
    query.bindValue(":totalProofOfWork", record._totalProofOfWork);

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool exists(QSqlDatabase db, const PK & pk, Record & r) {

    // Prepare select query
    QSqlQuery query(db);

    query.prepare("SELECT * FROM BlockHeader WHERE blockId = :blockId");

    // Bind values to query fields
    query.bindValue(":blockId", pk.toByteArray());

    query.exec();

    Q_ASSERT(query.lastError().type() == QSqlError::NoError);

    if(!query.first())
        return false;

    r = Record(query.record());

    Q_ASSERT(!query.next());

    return true;
}

bool exists(QSqlDatabase db, const PK & pk) {

    Record r;
    return exists(db, pk, r);
}

}
}
