/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_ADDRESS_HPP
#define WALLET_ADDRESS_HPP

#include <common/P2PKHAddress.hpp>

class QSqlQuery;
class QSqlDatabase;
class QSqlRecord;

namespace Wallet {
namespace Address {

    class Record {

    public:

        // Constructor from members
        Record(quint64 keyIndex, const Coin::P2PKHAddress & address);

        // Constructor from record
        Record(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

        // Getters and setters
        quint64 keyIndex() const;
        void setKeyIndex(quint64 keyIndex);

        Coin::P2PKHAddress address() const;
        void setAddress(const Coin::P2PKHAddress & address);

    private:

        // Index of wallet key to which this address corresponds
        quint64 _keyIndex;

        // Address
        Coin::P2PKHAddress _address;
    };

    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unboundedInsertQuery(QSqlDatabase db);

    // Lists all addresses in wallet
    QList<Record> allRecords(QSqlDatabase db);

}
}

#endif // WALLET_ADDRESS_HPP
