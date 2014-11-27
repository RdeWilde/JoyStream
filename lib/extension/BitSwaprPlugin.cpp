#include "BitSwaprPlugin.hpp"
#include "BitSwaprTorrentPlugin.hpp"

BitSwaprPlugin::BitSwaprPlugin(Controller * controller, QLoggingCategory * category)
    : controller_(controller)
    , session_(NULL)
    , category_(category == 0 ? QLoggingCategory::defaultCategory() : category) {

}

BitSwaprPlugin::~BitSwaprPlugin() {
    // No need to explicltly delete BitSwaprTorrentPlugin objects, since libtorrent has shared_ptr
}

Controller * BitSwaprPlugin::getController() {
    return controller_;
}

boost::shared_ptr<libtorrent::torrent_plugin> BitSwaprPlugin::new_torrent(libtorrent::torrent * newTorrent, void * userData) {

    // Create plugin
    BitSwaprTorrentPlugin * torrentPlugin = new BitSwaprTorrentPlugin(this, newTorrent, category_, BitSwaprTorrentPlugin::on);

    // Add to collection
    torrentPlugins.push_back(torrentPlugin);

    // Connect signals to controller slot
    //qRegisterMetaType<BitSwaprTorrentPlugin::TORRENT_MANAGEMENT_STATUS>();
    QObject::connect(torrentPlugin,
                     SIGNAL(torrentPluginStatus(int, int, BitSwaprTorrentPlugin::TORRENT_MANAGEMENT_STATUS, int, int)),
                     controller_,
                     SLOT(torrentPluginStatus(int, int, BitSwaprTorrentPlugin::TORRENT_MANAGEMENT_STATUS, int, int)));

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
