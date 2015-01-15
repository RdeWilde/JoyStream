#ifndef PEER_PLUGIN_STATE_HPP
#define PEER_PLUGIN_STATE_HPP

/*
 * Mutually exclusive set of states for peer plugin.
 *
 * Sharded!
 */

enum class PeerPluginState {
    started,
    BEP10_handshake_received, // BEP10 extension handshake
    observe_mode_announced,
    buy_mode_announced,
    sell_mode_announced,
    ended,

    // Buy States
    joined_contract,
    refund_signed_correctly,
    refund_signed_incorrectly,
    valid_piece_sent,
    invalid_piece_sent,
    to_slow,

    // Sell States
    invited_to_contract,
    refund_signature_requested,
    contract_ready,
    contract_invalid,
    contract_confirmation_deadline_expired,
    valid_payment_sent,
    invalid_payment_sent
};

#endif // PEER_PLUGIN_STATE_HPP
