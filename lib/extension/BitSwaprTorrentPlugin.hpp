
#ifndef BITSWAPR_TORRENT_PLUGIN_HPP
#define BITSWAPR_TORRENT_PLUGIN_HPP

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/socket.hpp> // tcp::endpoint

#include <boost/shared_ptr.hpp>

#include <QThread>

// Forward declaration
class BitSwaprPlugin;
class BitSwaprPeerPlugin;

class BitSwaprTorrentPlugin : public libtorrent::torrent_plugin {

private:

    // Parent plugin for BitSwapr
    BitSwaprPlugin * plugin_;

    // Torrent for this torrent_plugin
    libtorrent::torrent * torrent_;

    // Collection of peer plugin objects for each peer presently connected to this node through this torrent swarm
    std::vector<BitSwaprPeerPlugin *> peerPlugins;

public:

    // Constructor
    BitSwaprTorrentPlugin(BitSwaprPlugin * plugin, libtorrent::torrent * torrent);

    // Destructor
    //BitSwaprTorrentPlugin~();

    virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * peerConnection);
    virtual void on_piece_pass(int index);
    virtual void on_piece_failed(int index);
    virtual void tick();
    virtual bool on_resume();
    virtual bool on_pause();
    virtual void on_files_checked();
    virtual void on_state(int s);
    virtual void on_add_peer(libtorrent::tcp::endpoint const & tcpEndPoint, int src, int flags);
};

#endif
