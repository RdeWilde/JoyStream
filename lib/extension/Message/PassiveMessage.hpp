#ifndef PASSIVE_MESSAGE_HPP
#define PASSIVE_MESSAGE_HPP

#include "ExtendedMessage.hpp"

class PassiveMessage : public ExtendedMessage
{
public:

    /**
     * Inherited from ExtendedMessage
     */
    MessageType getMessageType() const;
    quint32 rawPayloadLength() const;
    void toRaw(const ExtendedMessageIdMapping & mapping, QDataStream & stream) const;
};

#endif // PASSIVE_MESSAGE_HPP
