/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_BLOCK_HEADER_HPP
#define WALLET_BLOCK_HEADER_HPP

#include <common/BlockId.hpp>
#include <common/TransactionMerkleTreeRoot.hpp>

#include <QDateTime>

class QSqlQuery;
class QSqlDatabase;
class QSqlRecord;

namespace Coin {
    class CoinBlockHeader;
}

namespace Wallet {
namespace BlockHeader {

    // Primary key
    typedef Coin::BlockId PK;

    struct Record {

        Record();
        Record(PK blockId,
                quint32 version,
                const Coin::BlockId & previousBlockId,
                const Coin::TransactionMerkleTreeRoot & merkleRoot,
                const QDateTime & timeStamp,
                quint32 nBits,
                quint32 nonce,
                quint64 transactionCount,
                bool isOnMainChain,
                quint32 totalProofOfWork);
        Record(const Coin::CoinBlockHeader & blockHeader,
               quint64 numberOfTransactions,
               bool isOnMainChain,
               quint32 totalProofOfWork);
        Record(const QSqlRecord & record);

        // Primary key: Block id of block to which this header corresponds
        PK _blockId;

        // Version field of block header
        quint32 _version;

        // Block id of block preceeding this one
        Coin::BlockId _previousBlockId;

        // Root of transaction merkle tree for block to which this header corresponds
        Coin::TransactionMerkleTreeRoot _merkleRoot;

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
        quint32 _totalProofOfWork;

    };

    // Table created, return true iff it worked
    bool createTable(QSqlDatabase db);

    // Insert record in table, return true iff it worked
    bool insert(QSqlDatabase db, const Record & record);

    // Checks whether record exists with given primary key, if so, it is written to r
    bool exists(QSqlDatabase db, const PK & pk, Record & r);
    bool exists(QSqlDatabase db, const PK & pk);

}
}

#endif // WALLET_BLOCK_HEADER_HPP
