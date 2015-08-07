/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef WALLET_KEY_HPP
#define WALLET_KEY_HPP

#include <common/CoinWrappers.hpp>
#include <QDateTime>

class QSqlQuery;
class QSqlDatabase;

class WalletKey
{
public:

    /**
    enum class Purpose {
        Receive,
        ContractChange,
        PayerInContractOutput,
        ContractFinal,
        PayeeInContractOutput,
        ContractPayment
    };
    */

    // Contructor from members
    WalletKey(quint64 index, const Coin::PrivateKey & privateKey, const QDateTime & generated);

    // Constructor from record
    // WalletKey(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery(QSqlDatabase db);

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery(QSqlDatabase db);

    // Conversion routines
    //static quint8 encodePurpose(Purpose purpose);
    //static Purpose decodePurpose(quint8 encoded);

    // Getters and setters
    quint64 index() const;
    void setIndex(quint64 index);

    //Purpose purpose() const;
    //void setPurpose(Purpose purpose);

    Coin::PrivateKey privateKey() const;
    void setPrivateKey(const Coin::PrivateKey & privateKey);

    QDateTime generated() const;
    void setGenerated(const QDateTime & generated);

private:

    // The sequence number is wallet
    quint64 _index;

    // Private key
    Coin::PrivateKey _privateKey;

    // Purpose for which key was generated
    //Purpose _purpose;

    // When key was generated
    QDateTime _generated;
};

#endif // WALLET_KEY_HPP
