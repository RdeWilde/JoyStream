#ifndef BUY_HPP
#define BUY_HPP

#include "ExtendedMessagePayload.hpp"

class Buy : public ExtendedMessagePayload
{
public:

    // Constructor based on members
    Buy(quint32 maxPrice, quint32 maxLock);

    // Constructor based on raw payload
    Buy(QDataStream & stream);

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

    // Getters
    quint32 maxPrice() const;
    quint32 maxLock() const;

private:

    // Buyer max price (satoshies)
    quint32 _maxPrice;

    // When refund becomes spendable at latest
    quint32 _maxLock;
};

#endif // BUY_HPP
