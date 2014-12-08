#include "Plugin.hpp"
#include "TorrentPlugin.hpp"
#include "controller/Controller.hpp" // needed for connecting
#include "TorrentPluginStatus.hpp" // needed for connecting

/*
#include <QMetaType>
Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)
*/
// #include <libtorrent/socket.hpp> // tcp::endpoint

Plugin::Plugin(Controller * controller, QLoggingCategory & category)
    : controller_(controller)
    , session_(NULL)
    , category_(category) {
}

Plugin::~Plugin() {
    // No need to explicltly delete TorrentPlugin objects, since libtorrent has shared_ptr
}

Controller * Plugin::getController() {
    return controller_;
}

boost::shared_ptr<libtorrent::torrent_plugin> Plugin::new_torrent(libtorrent::torrent * newTorrent, void * userData) {

    // Create plugin
    TorrentPlugin * torrentPlugin = new TorrentPlugin(this, newTorrent, category_, true);

    // Add to collection
    torrentPlugins_.insert(std::make_pair(newTorrent->info_hash(), torrentPlugin));

    // Connect torrent plugin signal
    /*
    //qRegisterMetaType<TorrentPluginStatus>();
    QObject::connect(torrentPlugin,
                     SIGNAL(updateTorrentPluginStatus(TorrentPluginStatus)),
                     controller_,
                     SLOT(updateTorrentPluginStatus(TorrentPluginStatus)));

    //qRegisterMetaType<libtorrent::tcp::endpoint>();

    QObject::connect(torrentPlugin,
                     SIGNAL(peerAdded(libtorrent::tcp::endpoint)),
                     controller_,
                     SLOT(extensionPeerAdded(libtorrent::tcp::endpoint)));

    QObject::connect(torrentPlugin,
                     SIGNAL(peerRemoved(libtorrent::tcp::endpoint)),
                     controller_,
                     SLOT(removePeer(libtorrent::tcp::endpoint)));
*/
    // Diagnostic
    qCDebug(category_) << "Torrent #" << torrentPlugins_.size() << " added.";

    // Return
    return boost::shared_ptr<libtorrent::torrent_plugin>(torrentPlugin);
}

void Plugin::added(libtorrent::aux::session_impl * session) {
    session_ = session;
}

void Plugin::on_alert(libtorrent::alert const * a) {

}

void Plugin::on_tick() {

}

bool Plugin::on_optimistic_unchoke(std::vector<libtorrent::policy::peer*> & peers) {
    return false;
}

void Plugin::save_state(libtorrent::entry & stateEntry) const {

}

void Plugin::load_state(libtorrent::lazy_entry const & stateEntry) {

}

void Plugin::removeTorrentPlugin(const libtorrent::sha1_hash & info_hash) {

}
