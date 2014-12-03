#ifndef SELLER_PEER_PLUGIN_HPP
#define SELLER_PEER_PLUGIN_HPP

#include "PeerPlugin.hpp"

class SellerPeerPlugin : public PeerPlugin
{
public:

    // Constructor
    SellerPeerPlugin(TorrentPlugin * torrentPlugin,
                     libtorrent::bt_peer_connection * bittorrentPeerConnection,
                     QLoggingCategory & category);

    virtual bool on_extended(int length, int msg, libtorrent::buffer::const_interval body);
    virtual void tick();
};

#endif // SELLER_PEER_PLUGIN_HPP
