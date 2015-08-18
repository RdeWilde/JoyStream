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

namespace Coin {
    class OutPoint;
}

namespace Wallet {
namespace OutPoint {

    class Record {

    public:

        struct PK {

            // Transaction id
            Coin::TransactionId _transactionId;
            // Had to use this nasty work around since mSIGNA is not type safe
            //unsigned char _hash[TXID_BYTE_LENGTH];

            // Index of transaction output
            quint32 _outputIndex;

        };

        /**
        // Constructor from members
        Record(const Coin::TransactionId & transactionId, quint32 outputIndex);
        */

        // Constructor from outpoints
        Record(const Coin::OutPoint & o);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

        // Getters and setters
        Coin::TransactionId transactionId() const;
        void setTransactionId(const Coin::TransactionId & transactionId);

        quint32 outputIndex() const;
        void setOutputIndex(quint32 outputIndex);

    private:

        /**
        // Transaction id
        Coin::TransactionId _transactionId;
        // Had to use this nasty work around since mSIGNA is not type safe
        //unsigned char _hash[TXID_BYTE_LENGTH];

        // Index of transaction output
        quint32 _outputIndex;
        */

        // Primary key
        PK _pk;
    };

    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);

}
}

#endif // WALLET_OUTPOINT

