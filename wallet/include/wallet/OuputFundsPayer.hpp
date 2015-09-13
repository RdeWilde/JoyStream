/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef WALLET_OUPUT_FUNDS_PAYER_HPP
#define WALLET_OUPUT_FUNDS_PAYER_HPP

#include <QByteArray>

class QSqlQuery;
class QSqlDatabase;

namespace Wallet {
namespace OuputFundsPayer { // Many-to-many association entity

    class Record {

    public:

        // Constructor from members
        Record(quint64 value, const QByteArray & scriptPubKey, quint64 _payerId);

        // Constructor from record
        // Record(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

    private:

        /**
         * Output PK
         */

        // Number of satoshies in output
        quint64 _value;

        // Serialized output script
        QByteArray _scriptPubKey;

        // dont think we need an index

        // Payer to which output funds
        quint64 _payerId;
    };

    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);

}
}

#endif // WALLET_OUPUT_FUNDS_PAYER_HPP

