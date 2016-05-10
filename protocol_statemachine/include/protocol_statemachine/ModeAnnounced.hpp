/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_MODEANNOUNCED_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_MODEANNOUNCED_HPP

namespace joystream {
namespace protocol_statemachine {

    enum class ModeAnnounced {
        none,
        observe,
        sell,
        buy
    };

}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_MODEANNOUNCED_HPP

