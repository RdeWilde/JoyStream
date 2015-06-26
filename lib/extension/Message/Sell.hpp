/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef SELL_HPP
#define SELL_HPP

#include "ExtendedMessagePayload.hpp"

#include <QDateTime>

class Sell : public ExtendedMessagePayload
{

public:

    // Default constructor
    Sell();

    // Constructor based on members
    Sell(quint32 minPrice, quint32 minLock, quint32 maxSellers);

    // Constructor based on raw data
    Sell(QDataStream & stream);

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

    // Getters and setters
    quint32 minPrice() const;
    void setMinPrice(quint32 minPrice);

    quint32 minLock() const;
    void setMinLock(quint32 minLock);

    quint32 maxSellers() const;
    void setMaxSellers(quint32 maxSellers);

private:

    // Piece price (in satoshi units)
    quint32 _minPrice;

    // When refund is spendable at the earliest
    quint32 _minLock;

    // Maximum number of sellers accepted in contract
    quint32 _maxSellers;
};

#endif // SELL_HPP
