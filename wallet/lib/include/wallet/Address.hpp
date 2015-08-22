/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_ADDRESS_HPP
#define WALLET_ADDRESS_HPP

#include <wallet/Key.hpp>
#include <common/P2PKHAddress.hpp>

class QSqlQuery;
class QSqlDatabase;
class QSqlRecord;

namespace Wallet {
namespace Address {

    // Primary key
    typedef Key::PK PK;

    struct Record {

        Record();
        Record(PK keyIndex, const Coin::P2PKHAddress & address);
        Record(const QSqlRecord & record);

        // Index of wallet key to which this address corresponds
        PK _keyIndex;

        // Address (is unique)
        Coin::P2PKHAddress _address;
    };

    bool createTable(QSqlDatabase db);
    bool insert(QSqlDatabase db, const Record & record);

    // Lists all addresses in wallet
    QList<Record> allRecords(QSqlDatabase db);

    // Checks whether record exists with given primary key, if so, it is written to r
    bool exists(QSqlDatabase db, const PK & pk, Record & r);
    bool exists(QSqlDatabase db, const PK & pk);

    // Finds record with given address, returns true IFF it exists, writes result into supplied record r
    bool findFromAddress(QSqlDatabase db, const Coin::P2PKHAddress & address, Record & r);
}
}

#endif // WALLET_ADDRESS_HPP
