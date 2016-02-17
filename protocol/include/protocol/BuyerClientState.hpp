/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUYER_CLIENT_STATE_HPP
#define JOYSTREAM_PROTOCOL_BUYER_CLIENT_STATE_HPP

namespace joystream {
namespace protocol {

    // State of client in buy mode
    enum class BuyerClientState {

        // We have not sent any message after extended handshake
        no_joystream_message_sent,

        // We have sent buyer mode message
        buyer_mode_announced,

        // We have sent join_contraact message
        invited_to_contract,

        // We ignored joining_contract message because the contract was full
        ignored_join_contract_from_peer,

        // We have invited peer to join payment channel, which may or may not have succeeded
        invited_to_join_payment_channel,

        // We invited to sign refund as a seller, but it took too long
        was_to_slow_to_sign_refund

        // something about not joining due to timeout????
    };

}
}


#endif // JOYSTREAM_PROTOCOL_BUYER_CLIENT_STATE_HPP
