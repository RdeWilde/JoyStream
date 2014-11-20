#include "BitSwaprPlugin.hpp"
#include "BitSwaprTorrentPlugin.hpp"

BitSwaprPlugin::BitSwaprPlugin(QLoggingCategory * category)
    : session_(NULL)
    , category_(category == 0 ? QLoggingCategory::defaultCategory() : category) {

}

BitSwaprPlugin::~BitSwaprPlugin() {

    // Iterate torrent plugins and delete them
    for(std::vector<BitSwaprTorrentPlugin *>::iterator i = torrentPlugins.begin(),
        end(torrentPlugins.end()); i != end;i++)
        delete *i;
}

boost::shared_ptr<libtorrent::torrent_plugin> BitSwaprPlugin::new_torrent(libtorrent::torrent * newTorrent, void * userData) {

    // Create plugin
    BitSwaprTorrentPlugin * torrentPlugin = new BitSwaprTorrentPlugin(this, newTorrent, category_);

    // Add to collection
    torrentPlugins.push_back(torrentPlugin);

    // Diagnostic
    qCDebug(CATEGORY) << "Torrent #" << torrentPlugins.size() << " added.";

    // Return
    return boost::shared_ptr<libtorrent::torrent_plugin>(torrentPlugin);
}

void BitSwaprPlugin::added(libtorrent::aux::session_impl * session) {
    session_ = session;
}

void BitSwaprPlugin::on_alert(libtorrent::alert const * a) {

}

void BitSwaprPlugin::on_tick() {

}

bool BitSwaprPlugin::on_optimistic_unchoke(std::vector<libtorrent::policy::peer*> & peers) {
    return false;
}

void BitSwaprPlugin::save_state(libtorrent::entry & stateEntry) const {

}

void BitSwaprPlugin::load_state(libtorrent::lazy_entry const & stateEntry) {

}
