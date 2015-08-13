/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Message/Observe.hpp>
#include <core/extension/Message/MessageType.hpp>

MessageType Observe::messageType() const {
    return MessageType::observe;
}
