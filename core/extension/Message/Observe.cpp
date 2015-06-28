/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "Observe.hpp"
#include "MessageType.hpp"

MessageType Observe::messageType() const {
    return MessageType::observe;
}
