/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef SELLER_MODE_PEER_ACTION_HPP
#define SELLER_MODE_PEER_ACTION_HPP

/**
 * Mutually exclusive set of state a peer can
 * announce to a sell mode client.
 *
 * Change name later!!!
 */
enum class SellModePeerAction {
    not_acted,
    extended_handshake_completed,
    observe_mode_announced,
    buy_mode_announced,
    sell_mode_announced,
    ended,



    invited_to_contract,
    refund_signature_requested,
    contract_ready,
    contract_invalid,

    valid_payment_sent,
    invalid_payment_sent
};

// Error states, make new enum later
// contract_confirmation_deadline_expired,
#endif // SELLER_MODE_PEER_ACTION_HPP
