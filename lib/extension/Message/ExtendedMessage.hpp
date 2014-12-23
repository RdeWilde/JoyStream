#ifndef EXTENDED_MESSAGE_HPP
#define EXTENDED_MESSAGE_HPP

#include <cstdint>

#include <QDataStream>

#include "ExtendedMessageIdMapping.hpp"

class ExtendedMessage
{
public:

    // Message factory based on only payload, header is encoded in type argument
    static ExtendedMessage * fromRaw(MessageType type, QDataStream & stream);

    /**
     * Virtual methods that subclassing messages have to implement:
     */

    // Get type of message
    virtual MessageType getMessageType() const = 0;

    // Length of payload of extended message (not full message, hence BEP10 id field not counted)
    virtual quint32 extendedPayloadLength() const = 0;

    // Length of full message, not only extended part
    quint32 length() const;

    // Write wire form of full message to stream
    virtual void wireForm(const ExtendedMessageIdMapping & mapping, QDataStream & stream) const = 0;

protected:

    // Writes to stream: [messageLength():uint32_t][(bt_peer_connection::msg_extended):uint8_t][id:uint8_t]
    void writeBEP10Header(QDataStream & stream, quint8 id) const;
};

#endif // EXTENDED_MESSAGE_HPP
