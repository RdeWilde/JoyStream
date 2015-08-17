/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_OUTPUT_HPP
#define WALLET_OUTPUT_HPP

#include <common/CoinWrappers.hpp>

#include <QByteArray>

class QSqlQuery;
class QSqlDatabase;

namespace Wallet {
namespace Output {

    class Record {

    public:

        // Constructor from members
        Record(quint64 value, const QByteArray & pubKeyScript, quint64 keyIndex);

        // Constructor from record
        // Output(const QSqlRecord & record);

        // Getters and setters
        quint64 value() const;
        void setValue(quint64 value);

        QByteArray pubKeyScript() const;
        void setPubKeyScript(const QByteArray & pubKeyScript);

        quint64 keyIndex() const;
        void setKeyIndex(quint64 keyIndex);

    private:

        // Number of satoshies in output
        quint64 _value;

        // Serialized output script
        QByteArray _pubKeyScript;

        // Address to which this address corresponds
        quint64 _keyIndex;
    };

    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);

}
}

#endif // WALLET_OUTPUT_HPP

