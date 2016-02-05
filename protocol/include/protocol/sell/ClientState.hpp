/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELL_CLIENT_STATE_HPP
#define JOYSTREAM_PROTOCOL_SELL_CLIENT_STATE_HPP

namespace joystream {
namespace protocol {
namespace sell {

    enum class ClientState {

        // We have not sent any message after extended handshake
        no_joystream_message_sent,

        // We have sent seller mode message
        seller_mode_announced,

        // We have sent joining contract message
        joined_contract,

        // We ignored contract invitation: Why would we ever do this?
        ignored_contract_invitation,

        // We signed refund
        signed_refund,

        // We ignored signing refund invitation: Why would we ever do this?
        ignored_sign_refund_invitation,

        // Waiting for first request, after the ready message was sent
        awaiting_fullpiece_request_after_ready_announced,

        // Waiting for payment
        awaiting_payment,

        // Waiting for next request, after a payment was made
        awaiting_piece_request_after_payment,

        // We are doing async reading of a piece from disk,
        // which occurs when when a valid full piece request arrives
        reading_piece_from_disk,

        // We are trying to get the last payment received confirmed
        trying_to_claim_last_payment,

        //
        last_payment_confirmed
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_SELL_CLIENT_STATE_HPP

