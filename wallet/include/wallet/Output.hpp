/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_OUTPUT_HPP
#define WALLET_OUTPUT_HPP

#include <QByteArray>
#include <QVariant>

class QSqlQuery;
class QSqlDatabase;
class QSqlRecord;

namespace Coin {
    class TxOut;
}

namespace Wallet {
namespace Output {

    struct PK {

        PK();
        PK(quint64 value, const QByteArray & scriptPubKey);
        PK(const Coin::TxOut & out);

        // Number of satoshies in output
        quint64 _value;

        // Serialized output script
        QByteArray _scriptPubKey;
    };

    struct Record {

        Record();
        Record(const PK & pk, const QVariant & keyIndex);
        Record(const QSqlRecord & record);

        // Primary key
        PK _pk;

        // Address to which this address corresponds
        // QVariant is used over quint64 to support NULL values
        // since column can hold null values
        //quint64 _keyIndex;
        QVariant _keyIndex;
    };

    // Creates table, returns true IFF it worked
    bool createTable(QSqlDatabase & db);

    // Insert
    bool insert(QSqlDatabase & db, const Record & record);

    // Whether record with given private key exists
    bool exists(QSqlDatabase & db, const PK & pk, Record & r);
    bool exists(QSqlDatabase & db, const PK & pk);

}
}

#endif // WALLET_OUTPUT_HPP
