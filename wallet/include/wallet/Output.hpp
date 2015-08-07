/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <common/CoinWrappers.hpp>

#include <QByteArray>

class QSqlQuery;
class QSqlDatabase;

class Output {

public:

    // Constructor from members
    Output(quint64 value, const QByteArray & pubKeyScript, quint64 walletKeyIndex);

    // Constructor from record
    // Output(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery(QSqlDatabase db);

    // Getters and setters
    quint64 value() const;
    void setValue(quint64 value);

    QByteArray pubKeyScript() const;
    void setPubKeyScript(const QByteArray & pubKeyScript);

    quint64 walletKeyIndex() const;
    void setWalletKeyIndex(quint64 walletKeyIndex);

private:

    // Number of satoshies in output
    quint64 _value;

    // Serialized output script
    QByteArray _pubKeyScript;

    // Address to which this address corresponds
    quint64 _walletKeyIndex;
};

#endif // OUTPUT_HPP

