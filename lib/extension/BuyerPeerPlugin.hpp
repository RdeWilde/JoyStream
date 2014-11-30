#ifndef BUYER_PEER_PLUGIN_HPP
#define BUYER_PEER_PLUGIN_HPP

#include "PeerPlugin.hpp"

class BuyerPeerPlugin : public PeerPlugin
{
public:

    // Constructor
    BuyerPeerPlugin(TorrentPlugin * torrentPlugin,
                    libtorrent::bt_peer_connection * bittorrentPeerConnection,
                    QLoggingCategory & category);

    virtual bool on_extended(int length, int msg, libtorrent::buffer::const_interval body);
};

#endif // BUYER_PEER_PLUGIN_HPP
