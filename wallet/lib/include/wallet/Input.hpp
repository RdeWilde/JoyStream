/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_INPUT_HPP
#define WALLET_INPUT_HPP

#include <wallet/OutPoint.hpp>

#include <QByteArray>

class QSqlDatabase;
class QSqlRecord;

namespace Coin {
    class TxIn;
}

namespace Wallet {
namespace Input {

    // Primary key
    struct PK {

        PK();
        PK(const OutPoint::PK & outPointPK,
           const QByteArray & scriptSig,
           quint32 sequence);
        PK(const Coin::TxIn & in);

        // OutPoint being spent by input
        OutPoint::PK _outPoint;

        // Serialized Input script
        QByteArray _scriptSig;

        // Input sequence number
        quint32 _sequence;

    };

    struct Record {

        Record();
        Record(const PK & pk);
        Record(const QSqlRecord & record);

        // Primary key
        PK _pk;
    };

    // Creates table, returns true IFF it worked
    bool createTable(QSqlDatabase db);

    // Insert record, returns true IFF it worked
    bool insert(QSqlDatabase db, const Record & record);

    // Checks whether record exists with given primary key, if so, it is written to r
    bool exists(QSqlDatabase & db, const PK & pk, Record & r);
    bool exists(QSqlDatabase & db, const PK & pk);

}
}

#endif // WALLET_INPUT_HPP
