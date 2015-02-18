#ifndef SELL_HPP
#define SELL_HPP

#include "ExtendedMessagePayload.hpp"

#include <QDateTime>

class Sell : public ExtendedMessagePayload
{

public:

    // Constructor based on members
    Sell(quint32 minPrice, QDateTime minLock);

    // Constructor based on raw data
    Sell(QDataStream & stream);

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

    // Getters
    quint32 minPrice() const;
    QDateTime minLock() const;

private:

    // Piece price (in satoshi units)
    quint32 _minPrice;

    // When refund is spendable at the earliest
    QDateTime _minLock;
};

#endif // SELL_HPP