/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 8 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_SESSIONMODE_HPP
#define JOYSTREAM_PROTOCOLSESSION_SESSIONMODE_HPP

namespace joystream {
namespace protocol_session {

    enum class SessionMode {
        NotSet,
        Observe,
        Buy,
        Sell
    };

}
}

#endif // JOYSTREAM_PROTOCOLSESSION_SESSIONMODE_HPP
