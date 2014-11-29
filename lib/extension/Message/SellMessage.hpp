#ifndef SELL_MESSAGE_HPP
#define SELL_MESSAGE_HPP

#include "ExtendedMessage.hpp"

class SellMessage : public ExtendedMessage
{

public:

    // Unit price
    quint32 price_;

    // Fee used
    quint32 fee_;

    // Minimum bond required
    quint32 minimum_;

    // Constructor based on fields
    SellMessage(quint32 price, quint32 fee, quint32 minimum);

    // Constructor based on raw data
    SellMessage(QDataStream & extendedPayloadStream);

    /**
     * Inherited from ExtendedMessage
     */
    Message::TYPE getMessageType() const;
    quint32 rawPayloadLength() const;
    void toRaw(const ExtendedMessageIdMapping & mapping, QDataStream & stream) const;

};

#endif // SELL_MESSAGE_HPP
