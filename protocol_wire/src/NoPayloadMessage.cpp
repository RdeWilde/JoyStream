/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/NoPayloadMessage.hpp>

namespace joystream {
namespace protocol_wire {

    quint32 NoPayloadMessage::length() const {
        return 0;
    }

    void NoPayloadMessage::write(QDataStream &) const {
        // Nothing to write
    }

}
}
