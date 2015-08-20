/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2015
 */

#ifndef WALLET_TRANSACTION_MINED_IN_BLOCK_HPP
#define WALLET_TRANSACTION_MINED_IN_BLOCK_HPP

#include <wallet/Transaction.hpp>
#include <wallet/BlockHeader.hpp>

#include <QByteArray>
#include <QSqlRecord>

namespace Wallet {
namespace TransactionMinedInBlock {

    struct PK {

        PK();
        PK(const Transaction::PK & transaction, const BlockHeader::PK & block);

        // Transaction
        Transaction::PK _transactionId;

        // Block
        BlockHeader::PK _blockId;
    };

    struct Record {

        Record();
        Record(const PK & pk, const QByteArray & merkleBranch, quint32 index);
        Record(const QSqlRecord & record);

        // Primary key
        PK _pk;

        // Merkle branch
        QByteArray _merkleBranch;

        // Position of transaction in block
        quint32 _index;
    };

    // Creates table
    bool createTable(QSqlDatabase db);

    // Insert record, returns true IFF it worked
    bool insert(QSqlDatabase db, const Record & record);

    // Checks whether record exists with given primary key, if so, it is written to r
    bool exists(QSqlDatabase & db, const PK & pk, Record & r);
    bool exists(QSqlDatabase & db, const PK & pk);

}
}

#endif // WALLET_TRANSACTION_MINED_IN_BLOCK_HPP

