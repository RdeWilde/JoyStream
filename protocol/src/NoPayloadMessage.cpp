/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol/NoPayloadMessage.hpp>

namespace joystream {
namespace protocol {

    quint32 NoPayloadMessage::length() const {
        return 0;
    }

    void NoPayloadMessage::write(QDataStream & stream) const {
        // Nothing to write
    }

}
}
