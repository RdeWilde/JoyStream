#ifndef SELL_MESSAGE_HPP
#define SELL_MESSAGE_HPP

#include "ExtendedMessagePayload.hpp"

#include <QDateTime>

class SellMessage : public ExtendedMessagePayload
{

public:

    // Constructor based on members
    SellMessage(quint32 minPrice, QDateTime minLock);

    // Constructor based on raw data
    SellMessage(QDataStream & stream);

    /**
     * Inherited from ExtendedMessagePayload
     */
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

#endif // SELL_MESSAGE_HPP
