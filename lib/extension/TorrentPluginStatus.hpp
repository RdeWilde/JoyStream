#ifndef TORRENT_PLUGIN_STATUS_HPP
#define TORRENT_PLUGIN_STATUS_HPP

#include <libtorrent/peer_id.hpp> // sha1_hash

class TorrentPluginStatus
{

public:

    // Constructor
    TorrentPluginStatus(libtorrent::sha1_hash & info_hash, int numberOfPeers, int numberOfPeersWithExtension, bool pluginOn, int tokensReceived, int tokensSent);

    // Default constructor, needed for Qt MOC
    TorrentPluginStatus();

    // Hash of torrent
    libtorrent::sha1_hash info_hash_;

    // Number of peers torrent plugin has been connected with (not all peers on libtorrent::torrent peer list?)
    int numberOfPeers_;

    // Among numberOfPeers_, how many did a correct BEP10 handshake indicating they supported BEP43
    int numberOfPeersWithExtension_;

    // Management mode of torrent plugin
    bool pluginOn_;

    // Number of tokens received during this session
    int tokensReceived_;

    // Numbre of tokens sent during this session
    int tokensSent_;

};

#include <QMetaType>
Q_DECLARE_METATYPE(TorrentPluginStatus)

#endif // TORRENT_PLUGIN_STATUS_HPP
