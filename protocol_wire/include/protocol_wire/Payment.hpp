/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_PAYMENT_HPP
#define JOYSTREAM_WIRE_PAYMENT_HPP

#include <common/Signature.hpp>

namespace joystream {
namespace protocol_wire {

    class Payment {

    public:

        // Defualt constructor
        Payment();

        // Constructor from members
        Payment(const Coin::Signature & sig);

        bool operator==(const Payment &) const;

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
