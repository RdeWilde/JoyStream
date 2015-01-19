#ifndef BUY_MODE_PEER_ACTION_HPP
#define BUY_MODE_PEER_ACTION_HPP

class enum BuyModePeerAction {

    not_acted,
    extended_handshake_completed,
    observe_mode_announced,
    buy_mode_announced,
    sell_mode_announced,
    ended,

    // Client is in Buy mode
    joined_contract,
    refund_signed_correctly,
    refund_signed_incorrectly,
    valid_piece_sent,
    invalid_piece_sent,
    to_slow,

    // Client is in Sell mode
    invited_to_contract,
    refund_signature_requested,
    contract_ready,
    contract_invalid,
    contract_confirmation_deadline_expired,
    valid_payment_sent,
    invalid_payment_sent

};

#endif // BUY_MODE_PEER_ACTION_HPP
