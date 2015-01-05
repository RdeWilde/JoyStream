#ifndef BUY_MESSAGE_HPP
#define BUY_MESSAGE_HPP

#include "ExtendedMessagePayload.hpp"

class BuyMessage : public ExtendedMessagePayload
{
public:

    // Constructor based on members
    BuyMessage(quint32 price, quint32 fee, qint32 btcVersion);

    // Constructor based on raw payload
    BuyMessage(QDataStream & stream);

    /**
     * Inherited from ExtendedMessage
     */
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

private:

    // Buyer max price (satoshies)
    quint32 _price;

    // Required fee (satoshies)
    quint32 _fee;

    // BitCoin transaction version number
    qint32 _btcVersion;
};

#endif // BUY_MESSAGE_HPP
