#ifndef PEER_PLUGIN_STATE_HPP
#define PEER_PLUGIN_STATE_HPP

/*
 * Different states of plugin by indicating
 * last arrived message
 */

enum class PeerPluginState {
    started,
    buy_message_received,
    sell_message_received
};

#endif // PEER_PLUGIN_STATE_HPP
