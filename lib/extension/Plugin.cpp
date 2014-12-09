#include "Plugin.hpp"
#include "TorrentPlugin.hpp"
#include "controller/Controller.hpp" // needed for connecting
#include "TorrentPluginStatus.hpp" // needed for connecting
#include "BuyerTorrentPlugin.hpp"
#include "SellerTorrentPlugin.hpp"

#include "TorrentPluginParameters.hpp"
#include "SellerTorrentPluginParameters.hpp"
#include "BuyerTorrentPluginParameters.hpp"

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

    // Create the appropriate torrent plugin depending on if we have full file
    TorrentPlugin * torrentPlugin;


    /**
     * Just putting in some random parameter stuff for it to build, all these params must be fetched from controller.
     */


    TorrentPluginParameters torrentPluginParameters(10,10,true);

    if(newTorrent->bytes_left() > 0)
        torrentPlugin = new BuyerTorrentPlugin(this, newTorrent, category_, true, torrentPluginParameters, BuyerTorrentPluginParameters(1,2));
    else
        torrentPlugin = new SellerTorrentPlugin(this, newTorrent, category_, true, torrentPluginParameters, SellerTorrentPluginParameters(5));

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
