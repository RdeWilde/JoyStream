/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUYERSESSIONSTATE_HPP
#define JOYSTREAM_PROTOCOL_BUYERSESSIONSTATE_HPP

namespace joystream {
namespace protocol {

    // State of session in buy mode
    struct BuyerSessionState {

        enum class State {

            active,

            stopped,

            paused
        };

        enum class Active {

            // Inviting sellers
            sending_invitations,

            // Requesting and downloading pieces
            downloading
        };

        BuyerSessionState()
            : _state(State::active)
            , _active(Active::sending_invitations ){
        }

        State _state;

        Active _active;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_BUYERSESSIONSTATE_HPP
