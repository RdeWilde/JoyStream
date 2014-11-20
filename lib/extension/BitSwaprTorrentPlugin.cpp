
#include "extension/BitSwaprTorrentPlugin.hpp"
#include "extension/BitSwaprPeerPlugin.hpp" // needed since we construct object

#include <iostream>

BitSwaprTorrentPlugin::BitSwaprTorrentPlugin(BitSwaprPlugin * plugin, libtorrent::torrent * torrent, QLoggingCategory * category)
    : plugin_(plugin)
    , torrent_(torrent)
    , category_(category == 0 ? QLoggingCategory::defaultCategory() : category) {

}

BitSwaprTorrentPlugin::~BitSwaprTorrentPlugin() {

    // Iterate torrent plugins and delete
    for(std::vector<BitSwaprPeerPlugin *>::iterator i = peerPlugins.begin(),
        end(peerPlugins.end());i != end;i++)
        delete *i;
}

boost::shared_ptr<libtorrent::peer_plugin> BitSwaprTorrentPlugin::new_connection(libtorrent::peer_connection * peerConnection) {

    // Create peer level plugin
    BitSwaprPeerPlugin * peerPlugin = new BitSwaprPeerPlugin(this, peerConnection);

    // Add to collection
    peerPlugins.push_back(peerPlugin);

    // Notify user
    qCDebug(CATEGORY) << "Peer #" << peerPlugins.size() << " added to torrent " << (libtorrent::to_hex(torrent_->info_hash().to_string())).c_str() << ".";

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
