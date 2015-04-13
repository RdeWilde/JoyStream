#ifndef BUY_HPP
#define BUY_HPP

#include "ExtendedMessagePayload.hpp"

class Buy : public ExtendedMessagePayload
{
public:

    // Default constructor
    Buy();

    // Constructor based on members
    Buy(quint32 maxPrice, quint32 maxLock, quint32 minSellers);

    // Constructor based on raw payload
    Buy(QDataStream & stream);

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

    // Getters and setters
    quint32 maxPrice() const;
    void setMaxPrice(quint32 maxPrice);

    quint32 maxLock() const;
    void setMaxLock(quint32 maxLock);

    quint32 minSellers() const;
    void setMinSellers(quint32 minSellers);

private:

    // Buyer max price (satoshies)
    quint32 _maxPrice;

    // When refund becomes spendable at latest
    quint32 _maxLock;

    // Minimum number of sellers in the contract
    quint32 _minSellers;
};

#endif // BUY_HPP
