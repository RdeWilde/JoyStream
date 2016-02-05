/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUY_SESSION_STATE_HPP
#define JOYSTREAM_PROTOCOL_BUY_SESSION_STATE_HPP

namespace joystream {
namespace protocol {
namespace buy {

    // State of session in buy mode
    enum class SessionState {

        // Need enough sellers to join
        waiting_for_full_set_of_sellers,

        // Requesting and downloading pieces
        downloading_pieces,

        // Have full torrent
        done
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_BUY_SESSION_STATE_HPP

