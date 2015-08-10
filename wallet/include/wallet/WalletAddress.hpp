/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_ADDRESS
#define WALLET_ADDRESS

//#include <wallet/WalletKey.hpp>
#include <common/P2PKHAddress.hpp>

class QSqlQuery;
class QSqlDatabase;

class WalletAddress {

public:

    // Constructor from members
    WalletAddress(quint64 walletKeyIndex, const Coin::P2PKHAddress & address);

    // Constructor from record
    // WalletAddress(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery(QSqlDatabase db);

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery(QSqlDatabase db);

    // Getters and setters
    quint64 walletKeyIndex() const;
    void setWalletKeyIndex(quint64 walletKeyIndex);

    Coin::P2PKHAddress address() const;
    void setAddress(const Coin::P2PKHAddress & address);

private:

    // Index of wallet key to which this address corresponds
    quint64 _walletKeyIndex;

    // Address
    Coin::P2PKHAddress _address;
};

#endif // WALLET_ADDRESS
