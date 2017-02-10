/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/Payment.hpp>
#include <protocol_wire/MessageType.hpp>

namespace joystream {
namespace protocol_wire {

    Payment::Payment() {
    }

    Payment::Payment(const Coin::Signature & sig)
        : _sig(sig) {
    }

    bool Payment::operator==(const Payment & rhs) const {
        return _sig == rhs.sig();
    }

    MessageType Payment::messageType() const {
        return MessageType::payment;
    }

    Coin::Signature Payment::sig() const {
        return _sig;
    }

    void Payment::setSig(const Coin::Signature & sig) {
        _sig = sig;
    }
}
}
