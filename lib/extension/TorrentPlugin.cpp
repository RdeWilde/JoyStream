
#include "TorrentPlugin.hpp"
#include "TorrentPluginConfiguration.hpp"
#include "PeerPlugin.hpp"
#include "PeerPluginConfiguration.hpp"
#include "PeerPluginStatus.hpp" // signal parameter
#include "controller/Controller.hpp" // needed to connect
#include "TorrentPluginRequest/TorrentPluginRequest.hpp"
#include "TorrentPluginRequest/SetConfigurationTorrentPluginRequest.hpp"

#include "Alert/TorrentPluginStatusAlert.hpp"

#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint

#include <QLoggingCategory>

TorrentPlugin::TorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, TorrentPluginConfiguration * torrentPluginConfiguration)
    : _plugin(plugin)
    , _torrent(torrent)
    , _category(category)
    , _pluginStarted(false)
    , _torrentPluginConfiguration(torrentPluginConfiguration) {

}

TorrentPlugin::~TorrentPlugin() {
    // Lets log, so we understand when libtorrent disposes of shared pointer
    qCDebug(_category) << "~TorrentPlugin() called.";
}

boost::shared_ptr<libtorrent::peer_plugin> TorrentPlugin::new_connection(libtorrent::peer_connection * peerConnection) {

    /**
     * Libtorrent docs (http://libtorrent.org/reference-Plugins.html#peer_plugin):
     * ===========================================================================
     * The peer_connection will be valid as long as the shared_ptr is being held by the
     * torrent object. So, it is generally a good idea to not keep a shared_ptr to
     * your own peer_plugin. If you want to keep references to it, use weak_ptr.
     */

    // Get end point to look up sets
    const libtorrent::tcp::endpoint & endPoint = peerConnection->remote();
    std::string endPointString = libtorrent::print_endpoint(endPoint);

    qCDebug(_category) << "New connection from" << endPointString.c_str();

    // Check if this peer should be accepted, if not
    // a null is returned, hence plugin is not installed
    if(installPluginOnNewConnection(peerConnection)) {

        qCDebug(_category) << "Rejected connection from peer, peer plugin not installed.";
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }

    // Recast as bittorrent peer connection
    libtorrent::bt_peer_connection * bittorrentPeerConnection = static_cast<libtorrent::bt_peer_connection*>(peerConnection);

    // Create peer plugin configuration
    //PeerPluginConfiguration * peerPluginConfiguration = createPeerPluginConfiguration(endPoint);

    // Create peer plugin
    PeerPlugin * peerPlugin = new PeerPlugin(this, bittorrentPeerConnection, _category); //, peerPluginConfiguration);

    // Add to plugin to collection
    _peerPlugins.insert(std::make_pair(endPoint, peerPlugin));

    // Add to torrent configuration if present
    //if(_torrentPluginConfiguration != NULL)
    //    _torrentPluginConfiguration->insertPeerPluginConfiguration(peerPluginConfiguration);

    // Connect peer plugin signal to controller slot
    //Controller * controller = plugin_->getController();
    //
    //QObject::connect(peerPlugin,
    //                 SIGNAL(peerPluginStatusUpdated(const PeerPluginStatus&)),
    //                 controller,
    //                 SLOT(updatePeerPluginStatus(const PeerPluginStatus&)));

    qCDebug(_category) << "Seller #" << _peerPlugins.size() << endPointString.c_str() << "added to " << _torrent->name().c_str();

    // Emit peer added signal
    // Should not be here, should be when a payment channel actually starts
    //emit peerAdded(peerPlugin->getPeerPluginId());

    // Return pointer to plugin as required
    return boost::shared_ptr<libtorrent::peer_plugin>(peerPlugin);
}

void TorrentPlugin::on_piece_pass(int index) {

}

void TorrentPlugin::on_piece_failed(int index) {

}

void TorrentPlugin::tick() {

    //qCDebug(_category) << "TorrentPlugin.tick()";

    // Send status signal
    sendTorrentPluginStatusSignal();
}

bool TorrentPlugin::on_resume() {
    return false;
}

bool TorrentPlugin::on_pause() {
    return false;
}

void TorrentPlugin::on_files_checked() {

}

void TorrentPlugin::on_state(int s) {

}

void TorrentPlugin::on_add_peer(libtorrent::tcp::endpoint const & endPoint, int src, int flags) {

    std::string endPointString = libtorrent::print_endpoint(endPoint);

    qCDebug(_category) << "Peer list extended with peer" << endPointString.c_str() << ": " << endPoint.port();

    // Check if we know from before that peer does not have
    if(_peersWithoutExtension.find(endPoint) != _peersWithoutExtension.end()) {

        qCDebug(_category) << "Not connecting to peer" << endPointString.c_str() << "which is known to not have extension.";
        return;
    }

    // Check if peer is banned due to irregular behaviour
    if(_irregularPeer.find(endPoint) != _irregularPeer.end()) {

        qCDebug(_category) << "Not connecting to peer" << endPointString.c_str() << "which has been banned due to irregular behaviour.";
        return;
    }

    // Try to connect to peer
    // Who owns this? I am allocatig on heap because I think connect_to_peer() requires persistent object?
    // ask on mailinglist.
    /*
    libtorrent::policy::peer peerPolicy = new libtorrent::policy::peer();

    torrent_->connect_to_peer(peerPolicy,true);
    */
}

bool TorrentPlugin::installPluginOnNewConnection(libtorrent::peer_connection * peerConnection) const {

    // If not configuratio is present, then install plugin
    if(_torrentPluginConfiguration == NULL)
        return true;

    // Get endpoint of connection
    const libtorrent::tcp::endpoint & endPoint = peerConnection->remote();

    // If we are using banning sets, then check this peer
    if(_torrentPluginConfiguration->getEnableBanningSets()) {

        // Check if we know from before that peer does not have
        if(_peersWithoutExtension.find(endPoint) != _peersWithoutExtension.end()) {
            qCDebug(_category) << "Peer is known to not have extension.";
            return false;
        }

        // Check if peer is banned due to irregular behaviour
        if(_irregularPeer.find(endPoint) != _irregularPeer.end()) {
            qCDebug(_category) << "Peer has been banned due to irregular behaviour.";
            return false;
        }
    }

    // Check that this is indeed a bittorrent client, and not a HTTP or URL seed, if not, then dont isntall
    if(peerConnection->type() != libtorrent::peer_connection::bittorrent_connection) {
        qCDebug(_category) << "Peer was not BitTorrent client.";
        return false;
    }

    // Otherwise install plugin
    return true;
}

/*
PeerPluginConfiguration * TorrentPlugin::createPeerPluginConfiguration(const libtorrent::tcp::endpoint & endPoint) const {

    // If plugin has not been started, or is in passive mode
    if(!_pluginStarted || _torrentPluginConfiguration == NULL)
        return NULL;
    else {

        // Do something more complicated here depending on being buyer or seller or whatever?
        // mode: _torrentPluginConfiguration->getPluginMode()

        // Basic
        return new PeerPluginConfiguration(endPoint, _torrentPluginConfiguration->getStartedPluginMode(), BEPSupportStatus::unknown, BEPSupportStatus::unknown, PeerPluginState::started);
    }
}
*/

bool TorrentPlugin::addToPeersWithoutExtensionSet(const libtorrent::tcp::endpoint & endPoint) {

    // Attempt to insert
    std::pair<std::set<libtorrent::tcp::endpoint>::iterator, bool> insertResult = _peersWithoutExtension.insert(endPoint);

    // Return whether object was actually inserted
    return insertResult.second;
}

bool TorrentPlugin::addToIrregularPeersSet(const libtorrent::tcp::endpoint & endPoint) {

    // Attempt to insert
    std::pair<std::set<libtorrent::tcp::endpoint>::iterator, bool> insertResult = _irregularPeer.insert(endPoint);

    // Return whether object was actually inserted
    return insertResult.second;
}

void TorrentPlugin::processTorrentPluginRequest(const TorrentPluginRequest * torrentPluginRequest) {

    qCDebug(_category) << "processTorrentPluginRequest";

    switch(torrentPluginRequest->getTorrentPluginRequestType()) {

        case TorrentPluginRequestType::SetConfiguration:

            processSetConfigurationTorrentPluginRequest(static_cast<const SetConfigurationTorrentPluginRequest *>(torrentPluginRequest));
            break;

        case TorrentPluginRequestType::SetPluginMode:

            //processSetPluginModeTorrentPluginRequest(static_cast<const SetPluginModeTorrentPluginRequest *>(torrentPluginRequest));
            break;
    }
}

void TorrentPlugin::processSetConfigurationTorrentPluginRequest(const SetConfigurationTorrentPluginRequest * setConfigurationTorrentPluginRequest) {

    /*
        !!!!what if we received this already once before??!!!

        for all peer plugin configurations in torrent configurations,

        -add to queue of critical endpoints you are trying to reach?
        -this also means that you should not just start buying and selling shit from others,
        you are in a special recovery mode

        -otherwise, if we didnt have any peers hanging arround, just gothrough
        _peerPlugin objects which have had all NULL configuration, give them
        basic configuration so it can change its operation.

        AVOID THIS WHOLE MESS BY SIMPLY NOT SAVING PEER CONFIGURATION FOR NOW
    */

    // We have now started
    _pluginStarted = true;

    // Get configuration by removing constness, maybee in future we send by value
    TorrentPluginConfiguration * torrentPluginConfiguration = const_cast<TorrentPluginConfiguration *>(setConfigurationTorrentPluginRequest->getTorrentPluginConfiguration());

    // Determine which one to use
    // if we got new one, use that one, if we didn
    if(torrentPluginConfiguration != NULL) {

        // Delete old configuration if we had one
        if(_torrentPluginConfiguration != NULL)
            delete _torrentPluginConfiguration;

        // Use new one
        _torrentPluginConfiguration = torrentPluginConfiguration;

    } else if(_torrentPluginConfiguration == NULL) {

        // serious problem
        _pluginStarted = false;

        qCDebug(_category) << "Disaster: Received NULL configuration, no configuration set from before.";

        return;
    }

    qCDebug(_category) << "Enabling peer plugins.";

    // Iterate peer plugins and set their configuration
    for(std::map<libtorrent::tcp::endpoint, PeerPlugin *>::iterator i = _peerPlugins.begin(),
            end(_peerPlugins.end()); i != end;i++) {

        // Get peer plugin
        PeerPlugin * peerPlugin = i->second;

        // Start plugin in given mode
        peerPlugin->startPlugin(_torrentPluginConfiguration->getStartedPluginMode());
    }
}

PeerPlugin * TorrentPlugin::getPeerPlugin(const libtorrent::tcp::endpoint & endPoint) {

    // Search for peer plugin
    std::map<libtorrent::tcp::endpoint, PeerPlugin *>::iterator i = _peerPlugins.find(endPoint);

    if(i == _peerPlugins.end())
        return NULL;
    else
        return i->second;
}

void TorrentPlugin::removePeerPlugin(PeerPlugin * plugin) {

    qCDebug(_category) << "TorrentPlugin::removePeerPlugin(): NOT IMPLEMENTED.";

    /*
    // Find iterator reference to plugin
    std::map<libtorrent::tcp::endpoint, PeerPlugin *>::iterator & mapIterator = _peerPlugins.find(plugin->getEndPoint());

    // Did we find match?
    if(mapIterator == _peerPlugins.end()) {
        qCDebug(_category) << "Could not find peer for removal.";
        return;
    }

    // Remove
    _peerPlugins.erase(mapIterator);

    // Delete object: Do we do this, or does libtorrent? and when is this safe?
    //delete mapIterator->second;

    // Emit peer added signal
    //emit peerRemoved(torrent_->info_hash(), mapIterator->first);
    */
}

/*
const libtorrent::sha1_hash & TorrentPlugin::getInfoHash() const {
    return _torrent->info_hash();
}
*/

void TorrentPlugin::sendTorrentPluginStatusSignal() {

    int numberOfPeers = _peerPlugins.size();

    int numberOfPeersWithExtension = 0;
    for(std::map<libtorrent::tcp::endpoint, PeerPlugin *>::iterator i = _peerPlugins.begin(),
        end(_peerPlugins.end()); i != end; i++) {

        // Count as supporting plugin if extended handshake was successful
        if((i->second)->getPeerBEP43SupportedStatus() == BEPSupportStatus::supported)
            numberOfPeersWithExtension++;
    }

    // Create torrent plugin alert
    TorrentPluginStatusAlert torrentPluginStatusAlert(_torrent->info_hash(), numberOfPeers, numberOfPeersWithExtension, _pluginStarted, 0, 0);

    // Send torrent plugin
    _plugin->sendAlertToSession(torrentPluginStatusAlert);
}

/*
const TorrentPluginConfiguration & TorrentPlugin::getTorrentPluginConfiguration() const {
    return _torrentPluginConfiguration;
}
*/
