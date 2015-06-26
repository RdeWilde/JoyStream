/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef NO_PAYLOAD_MESSAGE_HPP
#define NO_PAYLOAD_MESSAGE_HPP

#include "ExtendedMessagePayload.hpp"

// Abstract class used for all classes without payload
class NoPayloadMessage : public ExtendedMessagePayload
{
public:

    virtual MessageType messageType() const = 0;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;
};

#endif // NO_PAYLOAD_MESSAGE_HPP
