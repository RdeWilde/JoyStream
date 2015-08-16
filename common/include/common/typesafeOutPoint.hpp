/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_typesafeOUT_POINT_HPP
#define COIN_typesafeOUT_POINT_HPP

#include <common/TransactionId.hpp>

//class QJsonObject;

namespace Coin {

class typesafeOutPoint
{
public:

    // Default constructor
    typesafeOutPoint();

    // Constructor
    typesafeOutPoint(const TransactionId & transactionId, quint32 index);

    // Copy constructor
    typesafeOutPoint(const typesafeOutPoint& outputPoint);

    // Assignment operator
    typesafeOutPoint & operator=(const typesafeOutPoint& outputPoint);

    /**
    // Constructor from json
    OutPoint(const QJsonObject & json);

    // Encode as json
    QJsonObject json() const;
    */

    // Comparison for use with QMap
    friend bool operator==(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs);
    friend bool operator!=(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs);
    friend bool operator<(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs);

    // Flat representation, so outpoint can be used as json key
    typesafeOutPoint(const QString & string);

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

uint qHash(const Coin::typesafeOutPoint & o);

#endif // COIN_typesafeOUT_POINT_HPP
