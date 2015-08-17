/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/BlockHeader.hpp>

#include <QSqlQuery>

#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace BlockHeader {

Record::Record(const Coin::BlockId & blockId,
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

QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":blockId", _blockId.toByteArray());
    query.bindValue(":version", _version);
    query.bindValue(":previousBlockId", _previousBlockId.toByteArray());
    query.bindValue(":merkleRoot", _root.toByteArray());
    query.bindValue(":timeStamp", _timeStamp.toMSecsSinceEpoch());
    query.bindValue(":bits", _nBits);
    query.bindValue(":nonce", _nonce);
    query.bindValue(":transactionCount", _transactionCount);
    query.bindValue(":isOnMainChain", _isOnMainChain ? 1 : 0);
    query.bindValue(":totalProofOfWork", _totalProofOfWork);

    return query;
}

Coin::BlockId Record::blockId() const {
    return _blockId;
}

void Record::setBlockId(const Coin::BlockId & blockId) {
    _blockId = blockId;
}

quint32 Record::version() const {
    return _version;
}

void Record::setVersion(quint32 version) {
    _version = version;
}

Coin::BlockId Record::previousBlockId() const {
    return _previousBlockId;
}

void Record::setPreviousBlockId(const Coin::BlockId & previousBlockId) {
    _previousBlockId = previousBlockId;
}

Coin::TransactionMerkleTreeRoot Record::root() const {
    return _root;
}

void Record::setRoot(const Coin::TransactionMerkleTreeRoot & root) {
    _root = root;
}

QDateTime Record::timeStamp() const {
    return _timeStamp;
}

void Record::setTimeStamp(const QDateTime & timeStamp) {
    _timeStamp = timeStamp;
}

quint32 Record::nBits() const {
    return _nBits;
}

void Record::setNBits(quint32 nBits) {
    _nBits = nBits;
}

quint32 Record::nonce() const {
    return _nonce;
}

void Record::setNonce(quint32 nonce){
    _nonce = nonce;
}

quint64 Record::transactionCount() const {
    return _transactionCount;
}

void Record::setTransactionCount(quint64 numberOfTransactions) {
    _transactionCount = numberOfTransactions;
}

bool Record::isOnMainChain() const {
    return _isOnMainChain;
}

void Record::setIsOnMainChain(bool isOnMainChain) {
    _isOnMainChain = isOnMainChain;
}

quint32 Record::totalProofOfWork() const {
    return _totalProofOfWork;
}

void Record::setTotalProofOfWork(quint32 totalProofOfWork) {
    _totalProofOfWork = totalProofOfWork;
}

QSqlQuery createTableQuery(QSqlDatabase db) {

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
}

QSqlQuery unboundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO BlockHeader "
        "(blockId, version, previousBlockId, merkleRoot, timeStamp, bits, nonce, transactionCount, isOnMainChain, totalProofOfWork) "
    "VALUES "
        "(:blockId, :version, :previousBlockId, :merkleRoot, :timeStamp, :bits, :nonce, :transactionCount, :isOnMainChain, :totalProofOfWork) "
    );

    return query;
}

}
}
