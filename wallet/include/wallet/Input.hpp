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

namespace Wallet {
namespace Input {

    class Record {

    public:

        // Constructor for members
        Record(const OutPoint::Record & outPoint, const QByteArray & scriptSig, quint32 sequenc);



        // Constructor from record
        // Input(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

        // Getters and setters
        OutPoint::Record outPoint() const;
        void setOutPoint(const OutPoint::Record & outPoint);

        QByteArray scriptSig() const;
        void setScriptSig(const QByteArray & scriptSig);

        quint32 sequence() const;
        void setSequence(quint32 sequence);

    private:

        // OutPoint being spent by input
        OutPoint::Record _outPoint;

        // Serialized Input script
        QByteArray _scriptSig;

        // Input sequence number
        quint32 _sequence;

    };

    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);

}
}

#endif // WALLET_INPUT_HPP
