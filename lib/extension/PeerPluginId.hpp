#ifndef PEERPLUGIN_ID_HPP
#define PEERPLUGIN_ID_HPP

#include <libtorrent/peer.hpp> // sha1_hash
#include <libtorrent/socket.hpp> // libtorrent::tcp::endpoint

class PeerPluginId
{
public:

    // Default constructor required for Qt MOC
    PeerPluginId();

    // Constructor from members
    PeerPluginId(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint);

    // info hash of torrent to which this peer plugin corresponds
    libtorrent::sha1_hash info_hash_;

    // end point of peer
    libtorrent::tcp::endpoint endPoint_;
};

#include <QMetaType>
Q_DECLARE_METATYPE(PeerPluginId)

#endif // PEERPLUGIN_ID_HPP
