#ifndef PEER_PLUGIN_STATE_HPP
#define PEER_PLUGIN_STATE_HPP

/*
 * Mutually exclusive set of states for peer plugin
 * in terms of the last action of the peer.
 *
 * Sharded!
 */

enum class PeerPluginState {

    // Common
    started, // <-- what is this for again?

    BEP10_handshake_received, // BEP10 extension handshake
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

#endif // PEER_PLUGIN_STATE_HPP
