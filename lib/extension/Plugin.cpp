#include "Plugin.hpp"
#include "TorrentPlugin.hpp"
#include "TorrentPluginConfiguration.hpp"
#include "controller/Controller.hpp" // needed for connecting
#include "TorrentPluginStatus.hpp" // needed for connecting
#include "BuyerTorrentPlugin.hpp"
#include "SellerTorrentPlugin.hpp"

/*
#include <QMetaType>
Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)
*/
// #include <libtorrent/socket.hpp> // tcp::endpoint

Plugin::Plugin(Controller * controller, QLoggingCategory & category)
    : _controller(controller)
    , _session(NULL)
    , _category(category) {
}

Plugin::~Plugin() {
    // No need to explicltly delete TorrentPlugin objects, since libtorrent has shared_ptr

    // Delete TorrentPlugins!
    for(std::map<libtorrent::sha1_hash, TorrentPlugin *>::iterator i = _torrentPlugins.begin(),
            end(_torrentPlugins.end()); i != end; i++)
        delete i->second;
}

/*
Controller * Plugin::getController() {
    return _controller;
}
*/

boost::shared_ptr<libtorrent::torrent_plugin> Plugin::new_torrent(libtorrent::torrent * newTorrent, void * userData) {

    // Create torrent configuration
    TorrentPluginConfiguration torrentPluginConfiguration(PluginMode::NotDetermined, true, true);

    // Create the appropriate torrent plugin depending on if we have full file
    TorrentPlugin * torrentPlugin = new TorrentPlugin(this, newTorrent, _category, torrentPluginConfiguration);

    // Add to collection
    _torrentPlugins.insert(std::make_pair(newTorrent->info_hash(), torrentPlugin));

    /*
    // Connect torrent plugin signal
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
    qCDebug(_category) << "Torrent #" << _torrentPlugins.size() << " added.";

    // Return
    return boost::shared_ptr<libtorrent::torrent_plugin>(torrentPlugin);
}

void Plugin::added(libtorrent::aux::session_impl * session) {
    _session = session;
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
