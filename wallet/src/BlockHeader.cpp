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
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace BlockHeader {

Record::Record(){
}

Record::Record(PK blockId,
                quint32 version,
                const Coin::BlockId & previousBlockId,
                const Coin::TransactionMerkleTreeRoot & root,
                const QDateTime & timeStamp,
                quint32 nBits,
                quint32 nonce,
                quint64 numberOfTransactions,
                bool isOnMainChain,
                quint32 totalProofOfWork)
    : _blockId(blockId)
    , _version(version)
    , _previousBlockId(previousBlockId)
    , _root(root)
    , _timeStamp(timeStamp)
    , _nBits(nBits)
    , _nonce(nonce)
    , _transactionCount(numberOfTransactions)
    , _totalProofOfWork(totalProofOfWork) {
}

Record::Record(const Coin::CoinBlockHeader & h,
               quint64 numberOfTransactions,
               bool isOnMainChain,
               quint32 totalProofOfWork)
    : Record(h.getHashLittleEndian(),
             h.version(),
             h.prevBlockHash(),
             h.merkleRoot(),
             QDateTime::fromMSecsSinceEpoch(h.timestamp()),
             h.bits(),
             h.nonce(),
             numberOfTransactions,
             isOnMainChain,
             totalProofOfWork) {
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
    query.bindValue(":merkleRoot", record._root.toByteArray());
    query.bindValue(":timeStamp", record._timeStamp.toMSecsSinceEpoch());
    query.bindValue(":bits", record._nBits);
    query.bindValue(":nonce", record._nonce);
    query.bindValue(":transactionCount", record._transactionCount);
    query.bindValue(":isOnMainChain", record._isOnMainChain ? 1 : 0);
    query.bindValue(":totalProofOfWork", record._totalProofOfWork);

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}
/**
QSqlQuery unBoundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO BlockHeader "
        "(blockId, version, previousBlockId, merkleRoot, timeStamp, bits, nonce, transactionCount, isOnMainChain, totalProofOfWork) "
    "VALUES "
        "(:blockId, :version, :previousBlockId, :merkleRoot, :timeStamp, :bits, :nonce, :transactionCount, :isOnMainChain, :totalProofOfWork) "
    );

    return query;
}
*/

bool exists(QSqlDatabase db, const PK & pk, Record & r) {
    throw std::runtime_error("not implemented");
}

bool exists(QSqlDatabase db, const PK & pk) {

    Record r;
    return exists(db, pk, r);
}

}
}
