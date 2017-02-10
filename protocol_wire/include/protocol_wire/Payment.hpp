/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_PAYMENT_HPP
#define JOYSTREAM_WIRE_PAYMENT_HPP

#include <protocol_wire/ExtendedMessagePayload.hpp>
#include <common/Signature.hpp>

namespace joystream {
namespace protocol_wire {

    class Payment : public ExtendedMessagePayload {

    public:

        // Defualt constructor
        Payment();

        // Constructor from members
        Payment(const Coin::Signature & sig);

        virtual ~Payment() {}

        bool operator==(const Payment &) const;

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;

        // Getters and setters
        Coin::Signature sig() const;
        void setSig(const Coin::Signature & sig);

    private:

        // Buyer payment signature
        Coin::Signature _sig;
    };

}
}

#endif // JOYSTREAM_WIRE_PAYMENT_HPP
