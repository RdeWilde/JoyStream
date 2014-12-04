#ifndef EXTENDED_MESSAGE_HPP
#define EXTENDED_MESSAGE_HPP

#include <cstdint>

#include <QDataStream>

#include "ExtendedMessageIdMapping.hpp"

class ExtendedMessage
{
public:

    // Message factory based on only payload, header is encoded in type argument
    static ExtendedMessage * fromRaw(MessageType type, QDataStream & extendedPayloadStream);

    /**
     * Virtual methods that subclassing messages have to implement:
     */

    // Get type of message
    virtual MessageType getMessageType() const = 0;

    // Compute raw byte length of full extended message (1 byte header + payload)
    virtual quint32 rawPayloadLength() const = 0;

    // Write raw form of full extended message (header+payload) to buffer
    virtual void toRaw(const ExtendedMessageIdMapping & mapping, QDataStream & extendedMessageStream) const = 0;

};

#endif // EXTENDED_MESSAGE_HPP
