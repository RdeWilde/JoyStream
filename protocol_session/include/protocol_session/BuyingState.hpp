/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_BUYERSESSIONSTATE_HPP
#define JOYSTREAM_PROTOCOLSESSION_BUYERSESSIONSTATE_HPP

namespace joystream {
namespace protocol_session {

    // State of session in buy mode
    struct BuyingState {

        enum class State {

            // Only sends mode messages and monitors connections,
            // and resets all connections
            inactive,

            // Going through the motions
            active,

            // Same as inactive, but we will honor last payment,
            // and does not reset connections, hence we can go back to active later
            paused
        };

        enum class Active {

            // Inviting sellers
            sending_invitations,

            // Requesting and downloading pieces
            downloading
        };

        BuyingState()
            : _state(State::active)
            , _active(Active::sending_invitations ){
        }

        State _state;

        Active _active;
    };

}
}

#endif // JOYSTREAM_PROTOCOLSESSION_BUYERSESSIONSTATE_HPP
