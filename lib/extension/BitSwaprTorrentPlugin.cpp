
#include "extension/BitSwaprTorrentPlugin.hpp"
#include "extension/BitSwaprPeerPlugin.hpp" // needed since we construct object

#include <libtorrent/error_code.hpp>

#include <iostream>

BitSwaprTorrentPlugin::BitSwaprTorrentPlugin(BitSwaprPlugin * plugin, libtorrent::torrent * torrent, QLoggingCategory * category)
    : plugin_(plugin)
    , torrent_(torrent)
    , category_(category == 0 ? QLoggingCategory::defaultCategory() : category) {

}

BitSwaprTorrentPlugin::~BitSwaprTorrentPlugin() {

    // No need to explicltly delete BitSwaprPeerPlugin, since libtorrent has shared_ptr
}

// Returns plugin
BitSwaprPlugin * BitSwaprTorrentPlugin::getPlugin() {
    return plugin_;
}

boost::shared_ptr<libtorrent::peer_plugin> BitSwaprTorrentPlugin::new_connection(libtorrent::peer_connection * peerConnection) {

    // Create peer level plugin
    BitSwaprPeerPlugin * peerPlugin = new BitSwaprPeerPlugin(this, peerConnection);

    // Add to collection
    peerPlugins.push_back(peerPlugin);

    // Notify
    libtorrent::error_code ec;
    libtorrent::address const & addr = peerConnection->remote().address();
    const char * peerAddress = NULL;

    if (addr.is_v6())
        peerAddress = addr.to_string(ec).c_str();
    else
        peerAddress = addr.to_string(ec).c_str();

    if(ec) {
        qCCritical(CATEGORY) << ec.message().c_str();
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }

    short port = peerConnection->remote().port();

    //qCDebug(CATEGORY) << "Peer #" << peerPlugins.size() << "[" << peerAddress << ":" << port << "] added to torrent " << (libtorrent::to_hex(torrent_->info_hash().to_string())).c_str() << ".";

    // Return pointer as required
    return boost::shared_ptr<libtorrent::peer_plugin>(peerPlugin);
}

void BitSwaprTorrentPlugin::on_piece_pass(int index) {

}

void BitSwaprTorrentPlugin::on_piece_failed(int index) {

}

void BitSwaprTorrentPlugin::tick() {

}

bool BitSwaprTorrentPlugin::on_resume() {
    return false;
}

bool BitSwaprTorrentPlugin::on_pause() {
    return false;
}

void BitSwaprTorrentPlugin::on_files_checked() {

}

void BitSwaprTorrentPlugin::on_state(int s) {

}

void BitSwaprTorrentPlugin::on_add_peer(libtorrent::tcp::endpoint const & tcpEndPoint, int src, int flags) {

}
