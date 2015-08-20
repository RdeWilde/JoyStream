/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_OUTPOINT
#define WALLET_OUTPOINT

#include <common/TransactionId.hpp>
#include <QtGlobal> // quint32

class QSqlQuery;
class QSqlDatabase;
class QSqlRecord;

namespace Coin {
    class OutPoint;
}

namespace Wallet {
namespace OutPoint {

    struct PK {

        PK();
        PK(const Coin::TransactionId & transactionId, quint32 outputIndex);
        PK(const Coin::OutPoint & o);

        // Transaction id
        Coin::TransactionId _transactionId;
        // Had to use this nasty work around since mSIGNA is not type safe
        //unsigned char _hash[TXID_BYTE_LENGTH];

        // Index of transaction output
        quint32 _index;
    };

    struct Record {

        Record();
        Record(const PK & pk);
        Record(const QSqlRecord & record);

        // Primary key
        PK _pk;
    };

    // Create table, returns true IFF it worked
    bool createTable(QSqlDatabase db);

    // Insert into table, returns true IFF it worked
    bool insert(QSqlDatabase db, const Record & record);

    // Checks whether record exists with given primary key, if so, it is written to r
    bool exists(QSqlDatabase db, const PK & pk, Record & r);
    bool exists(QSqlDatabase db, const PK & pk);

}
}

#endif // WALLET_OUTPOINT

