#ifndef BUY_MESSAGE_HPP
#define BUY_MESSAGE_HPP

#include "ExtendedMessage.hpp"

class BuyMessage : public ExtendedMessage
{
public:

    /**
     * Inherited from ExtendedMessage
     */
    MessageType getMessageType() const;
    quint32 rawPayloadLength() const;
    void toRaw(const ExtendedMessageIdMapping & mapping, QDataStream & stream) const;
};

#endif // BUY_MESSAGE_HPP
