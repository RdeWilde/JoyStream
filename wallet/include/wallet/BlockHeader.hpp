/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef BLOCK_HEADER_HPP
#define BLOCK_HEADER_HPP

#include <common/BlockId.hpp>
#include <common/TransactionMerkleTreeRoot.hpp>

#include <QDateTime>

class QSqlQuery;
class QSqlDatabase;

namespace Wallet {
namespace BlockHeader {

    class Record {

    public:

        // Contructor from members
        Record(const Coin::BlockId & blockId,
                    quint32 version,
                    const Coin::BlockId & previousBlockId,
                    const Coin::TransactionMerkleTreeRoot & root,
                    const QDateTime & timeStamp,
                    quint32 nBits,
                    quint32 nonce,
                    quint64 transactionCount,
                    bool isOnMainChain,
                    quint32 totalProofOfWork);

        // Constructor from record
        // BlockHeader(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

        // Getters and setters
        Coin::BlockId blockId() const;
        void setBlockId(const Coin::BlockId & blockId);

        quint32 version() const;
        void setVersion(quint32 version);

        Coin::BlockId previousBlockId() const;
        void setPreviousBlockId(const Coin::BlockId & previousBlockId);

        Coin::TransactionMerkleTreeRoot root() const;
        void setRoot(const Coin::TransactionMerkleTreeRoot &root);

        QDateTime timeStamp() const;
        void setTimeStamp(const QDateTime & timeStamp);

        quint32 nBits() const;
        void setNBits(quint32 nBits);

        quint32 nonce() const;
        void setNonce(quint32 nonce);

        quint64 transactionCount() const;
        void setTransactionCount(quint64 transactionCount);

        bool isOnMainChain() const;
        void setIsOnMainChain(bool isOnMainChain);

        quint32 totalProofOfWork() const;
        void setTotalProofOfWork(quint32 totalProofOfWork);

    private:

        // Block id of block to which this header corresponds
        Coin::BlockId _blockId;

        // Version field of block header
        quint32 _version;

        // Block id of block preceeding this one
        Coin::BlockId _previousBlockId;

        // Root of transaction merkle tree for block to which this header corresponds
        Coin::TransactionMerkleTreeRoot _root;

        // Block creation time stamp (nTime)
        QDateTime _timeStamp;

        // The calculated difficulty target being used for this block (nBits)
        quint32 _nBits;

        // The nonce used to generate this block... to allow variations of the header and compute different hashes.
        quint32 _nonce;

        // The number of transactions in block to which this header corresponds
        quint64 _transactionCount;

        // Whether header corresponds to block currently on main chain
        // (look closer into this later)
        bool _isOnMainChain;

        // Total proof of work on all blocks up to and including this one
        //
        quint32 _totalProofOfWork;

    };

    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unboundedInsertQuery(QSqlDatabase db);

}
}

#endif // BLOCK_HEADER_HPP
