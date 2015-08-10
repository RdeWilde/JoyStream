/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef OUT_POINT_HPP
#define OUT_POINT_HPP

#include <common/TransactionId.hpp>

//class QJsonObject;

namespace Coin {

class OutPoint
{
public:

    // Default constructor
    OutPoint();

    // Constructor
    OutPoint(const TransactionId & transactionId, quint32 index);

    // Copy constructor
    OutPoint(const OutPoint& outputPoint);

    // Assignment operator
    OutPoint & operator=(const OutPoint& outputPoint);

    /**
    // Constructor from json
    OutPoint(const QJsonObject & json);

    // Encode as json
    QJsonObject json() const;
    */

    // Comparison for use with QMap
    friend bool operator==(const OutPoint & lhs, const OutPoint & rhs);
    friend bool operator!=(const OutPoint & lhs, const OutPoint & rhs);
    friend bool operator<(const OutPoint & lhs, const OutPoint & rhs);

    // Flat representation, so outpoint can be used as json key
    OutPoint(const QString & string);
    QString toString() const;

    // Getters and setters
    TransactionId transactionId() const;
    void setTransactionId(const TransactionId & transactionId);

    quint32 index() const;
    void setIndex(quint32 index);

private:

    // Transaction id of transaction
    TransactionId _txId;

    // Output index
    quint32 _index;
};

}

uint qHash(const Coin::OutPoint & o);

#endif // OUT_POINT_HPP
