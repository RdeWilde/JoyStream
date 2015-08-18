/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_OUTPUT_HPP
#define WALLET_OUTPUT_HPP

#include <QByteArray>

class QSqlQuery;
class QSqlDatabase;

namespace Wallet {
namespace Output {

    struct Record {

        struct PK {

            PK();
            PK(quint64 value, const QByteArray & pubKeyScript);

            // Number of satoshies in output
            quint64 _value;

            // Serialized output script
            QByteArray _pubKeyScript;
        };

        Record();
        Record(const PK & pk, quint64 keyIndex);

        // Output(const QSqlRecord & record);

        // Prepared insert query
        QSqlQuery insertQuery(QSqlDatabase db);

        // Primary key
        PK _pk;

        // Address to which this address corresponds
        // QVariant is used over quint64 to support null values
        // since column can hold null values
        quint64 _keyIndex;
    };

    // Query which creates table corresponding to entity
    QSqlQuery createTable(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);

    // Whether record with given private key exists
    bool exists(QSqlDatabase & db, const Record::PK & pk, Record & r);
    bool exists(QSqlDatabase & db, const Record::PK & pk);

}
}

#endif // WALLET_OUTPUT_HPP

