/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUY_CLIENT_STATE_HPP
#define JOYSTREAM_PROTOCOL_BUY_CLIENT_STATE_HPP

namespace joystream {
namespace protocol {
namespace buy {

    // State of client in buy mode
    enum class ClientState {

        // We have not sent any message after extended handshake
        no_joystream_message_sent,

        // We have sent buyer mode message
        buyer_mode_announced,

        // We have sent join_contraact message
        invited_to_contract,

        // We ignored joining_contract message because the contract was full
        ignored_join_contract_from_peer,

        // We have sent sign_refund message
        asked_for_refund_signature,

        // Postponed sendining ready message since not all signatures were ready
        received_valid_refund_signature_and_waiting_for_others,

        // Sent ready message
        // NOT REALLY NEEDED, WE NEVER WAIT FOR ANYTHIN AFTER THIS
        //announced_ready,

        // At least one request message has been sent, for which no piece message
        // has yet been returned
        //waiting_for_requests_to_be_serviced,

        // Not been assigned piece
        needs_to_be_assigned_piece,

        // We have requested a full piece, and we are waiting for it to arrive
        waiting_for_full_piece,

        // We are waiting for libtorrent to fire on_piece_pass() or on_piece_failed()
        // on a full piece which was recently received
        waiting_for_to_validate_and_store_piece
    };

}
}
}


#endif // JOYSTREAM_PROTOCOL_BUY_CLIENT_STATE_HPP

