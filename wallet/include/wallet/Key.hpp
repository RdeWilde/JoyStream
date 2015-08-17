/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef WALLET_KEY_HPP
#define WALLET_KEY_HPP

#include <common/PrivateKey.hpp>
#include <QDateTime>

class QSqlQuery;
class QSqlDatabase;

namespace Wallet {
namespace Key {

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

    class Record {

    public:

        // Contructor from members
        Record(quint64 index, const Coin::PrivateKey & sk, const QDateTime & generated, bool issued);

        // Constructor from record
        // Record(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

        // Getters and setters
        quint64 index() const;
        void setIndex(quint64 index);

        //Purpose purpose() const;
        //void setPurpose(Purpose purpose);

        Coin::PrivateKey sk() const;
        void setSk(const Coin::PrivateKey & sk);

        QDateTime generated() const;
        void setGenerated(const QDateTime & generated);

        bool issued() const;
        void setIssued(bool issued);

    private:

        // The (hd) index number is wallet
        quint64 _index;

        // Private key
        Coin::PrivateKey _sk;

        // Purpose for which key was generated
        //Purpose _purpose;

        // When key was generated
        QDateTime _generated;

        // Has key been issued,
        // that is been returned to wallet user in any way.
        // An issued may still not actually have been used, but
        // it should still not be reissued (
        bool _issued;

    };

    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unboundedInsertQuery(QSqlDatabase db);

    // Finds biggest index in table, throws exception if
    // table is empty
    quint64 maxIndex(QSqlDatabase db);

    // Counts the number of keys in the wallet
    quint64 numberOfKeysInWallet(QSqlDatabase db);

    // Conversion routines
    //static quint8 encodePurpose(Purpose purpose);
    //static Purpose decodePurpose(quint8 encoded);
}
}

#endif // WALLET_KEY_HPP
