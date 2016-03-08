/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_READY_HPP
#define JOYSTREAM_PROTOCOL_WIRE_READY_HPP

#include <protocol/wire/ExtendedMessagePayload.hpp>
#include <common/typesafeOutPoint.hpp>

namespace joystream {
namespace protocol {
namespace wire {

    class Ready : public ExtendedMessagePayload {

    public:

        Ready();

        Ready(const Coin::typesafeOutPoint & anchor);

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual quint32 length() const;
        virtual void write(QDataStream & stream) const;

        // Getters and setters
        Coin::typesafeOutPoint anchor() const;
        void setAnchor(const Coin::typesafeOutPoint & anchor);

    private:

        // Anchor for contract
        Coin::typesafeOutPoint _anchor;
    };

}
}
}
#endif // JOYSTREAM_PROTOCOL_WIRE_READY_HPP
