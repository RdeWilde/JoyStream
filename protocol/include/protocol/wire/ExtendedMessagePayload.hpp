/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_EXTENDED_MESSAGE_PAYLOAD_HPP
#define JOYSTREAM_PROTOCOL_WIRE_EXTENDED_MESSAGE_PAYLOAD_HPP

#include <QtGlobal> // quint32

class QDataStream;

namespace joystream {
namespace protocol {
namespace wire {

    enum class MessageType;

    class ExtendedMessagePayload {

    public:

        // Message factory based on only payload, header is encoded in type argument
        static ExtendedMessagePayload * fromRaw(MessageType type, QDataStream & stream, int lengthOfExtendedMessagePayload);

        /**
         * Virtual methods that subclassing messages have to implement
         */

        // Get type of message
        virtual MessageType messageType() const = 0;

        // Length of payload of extended message (not full message, hence BEP10 id field not counted)
        virtual quint32 length() const = 0;

        // Write wire form of extended message payload (not payload of full message, hence BEP10 header not written)
        virtual void write(QDataStream & stream) const = 0;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_EXTENDED_MESSAGE_PAYLOAD_HPP
