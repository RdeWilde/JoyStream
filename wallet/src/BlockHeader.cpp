/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/BlockHeader.hpp>

#include <QSqlQuery>

#include <QVariant> // QSqlQuery::bind needs it

BlockHeader::BlockHeader(const Coin::BlockId & blockId,
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

QSqlQuery BlockHeader::createTableQuery(QSqlDatabase db) {

    return QSqlQuery("\
    CREATE TABLE BlockHeader (\
        blockId             BLOB        NOT NULL,\
        version             INTEGER     NOT NULL,\
        previousBlockId     BLOB        NOT NULL,\
        merkleRoot          BLOB        NOT NULL,\
        timeStamp           INTEGER     NOT NULL,\
        bits                INTEGER     NOT NULL,\
        nonce               INTEGER     NOT NULL,\
        transactionCount    INTEGER     NOT NULL,\
        isOnMainChain       INTEGER     NOT NULL,\
        totalProofOfWork    INTEGER     NOT NULL,\
        PRIMARY KEY(blockId)\
        UNIQUE(version, previousBlockId, merkleRoot, timeStamp, bits, nonce) ,\
    )", db);
}

QSqlQuery BlockHeader::unboundedInsertQuery(QSqlDatabase db) {

    return QSqlQuery("\
                     INSERT INTO BlockHeader \
                     (blockId, version, previousBlockId, merkleRoot, timeStamp, bits, nonce, transactionCount, isOnMainChain, totalProofOfWork)\
                     VALUES\
                     (:blockId, :version, :previousBlockId, :merkleRoot, :timeStamp, :bits, :nonce, :transactionCount, :isOnMainChain, :totalProofOfWork)\
                     ", db);
}

QSqlQuery BlockHeader::insertQuery(QSqlDatabase db) {

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

Coin::BlockId BlockHeader::blockId() const {
    return _blockId;
}

void BlockHeader::setBlockId(const Coin::BlockId & blockId) {
    _blockId = blockId;
}

quint32 BlockHeader::version() const {
    return _version;
}

void BlockHeader::setVersion(quint32 version) {
    _version = version;
}

Coin::BlockId BlockHeader::previousBlockId() const {
    return _previousBlockId;
}

void BlockHeader::setPreviousBlockId(const Coin::BlockId & previousBlockId) {
    _previousBlockId = previousBlockId;
}

Coin::TransactionMerkleTreeRoot BlockHeader::root() const {
    return _root;
}

void BlockHeader::setRoot(const Coin::TransactionMerkleTreeRoot & root) {
    _root = root;
}

QDateTime BlockHeader::timeStamp() const {
    return _timeStamp;
}

void BlockHeader::setTimeStamp(const QDateTime & timeStamp) {
    _timeStamp = timeStamp;
}

quint32 BlockHeader::nBits() const {
    return _nBits;
}

void BlockHeader::setNBits(quint32 nBits) {
    _nBits = nBits;
}

quint32 BlockHeader::nonce() const {
    return _nonce;
}

void BlockHeader::setNonce(quint32 nonce){
    _nonce = nonce;
}

quint64 BlockHeader::transactionCount() const {
    return _transactionCount;
}

void BlockHeader::setTransactionCount(quint64 numberOfTransactions) {
    _transactionCount = numberOfTransactions;
}

bool BlockHeader::isOnMainChain() const {
    return _isOnMainChain;
}

void BlockHeader::setIsOnMainChain(bool isOnMainChain) {
    _isOnMainChain = isOnMainChain;
}

quint32 BlockHeader::totalProofOfWork() const {
    return _totalProofOfWork;
}

void BlockHeader::setTotalProofOfWork(quint32 totalProofOfWork) {
    _totalProofOfWork = totalProofOfWork;
}
