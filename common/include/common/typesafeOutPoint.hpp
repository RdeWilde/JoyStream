/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_typesafeOUT_POINT_HPP
#define COIN_typesafeOUT_POINT_HPP

#include <common/TransactionId.hpp>

namespace Coin {

class OutPoint;

class typesafeOutPoint
{
public:

    // Default constructor
    typesafeOutPoint();

    // Constructor
    typesafeOutPoint(const TransactionId & transactionId, quint32 index);

    // Copy constructor
    typesafeOutPoint(const typesafeOutPoint& outPoint);

    typesafeOutPoint(const OutPoint & outPoint);

    // Serialized length
    static uint length();

    // Assignment operator
    typesafeOutPoint & operator=(const typesafeOutPoint& outputPoint);

    // Comparison for use with QMap
    friend bool operator==(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs);
    friend bool operator!=(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs);
    friend bool operator<(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs);

    // Stream i/o operators
    friend QDataStream & operator<<(QDataStream & stream, const typesafeOutPoint &);
    friend QDataStream & operator>>(QDataStream & stream, typesafeOutPoint &);

    // Flat representation, so outpoint can be used as json key
    typesafeOutPoint(const QString & string);

    QString toLittleEndianTxIdString() const;

    Coin::OutPoint getClassicOutPoint() const;

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
