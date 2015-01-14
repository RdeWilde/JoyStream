#ifndef BUY_MESSAGE_HPP
#define BUY_MESSAGE_HPP

#include "ExtendedMessagePayload.hpp"

#include <QDateTime>

class BuyMessage : public ExtendedMessagePayload
{
public:

    // Constructor based on members
    BuyMessage(quint32 maxPrice, QDateTime maxLock);

    // Constructor based on raw payload
    BuyMessage(QDataStream & stream);

    /**
     * Inherited from ExtendedMessage
     */
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

    // Getters
    quint32 maxPrice() const;
    QDateTime maxLock() const;

private:

    // Buyer max price (satoshies)
    quint32 _maxPrice;

    // When refund becomes spendable at latest
    QDateTime _maxLock;
};

#endif // BUY_MESSAGE_HPP
