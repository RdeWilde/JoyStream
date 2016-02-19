/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUYER_SESSION_STATE_HPP
#define JOYSTREAM_PROTOCOL_BUYER_SESSION_STATE_HPP

namespace joystream {
namespace protocol {

    // State of session in buy mode
    enum class BuyerSessionState {

        // The number of sellers not yet set
        waiting_to_decide_how_many_sellers_to_have,

        // Need enough sellers to join by si
        waiting_for_full_set_of_sellers_with_signed_refund,

        // Requesting and downloading pieces
        downloading_pieces,

        // Have full torrent
        done
    };

}
}

#endif // JOYSTREAM_PROTOCOL_BUYER_SESSION_STATE_HPP
