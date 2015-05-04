﻿#include "TorrentPlugin.hpp"
//#include "TorrentPluginConfiguration.hpp"
#include "PeerPlugin.hpp"
//#include "PeerPluginConfiguration.hpp"
//#include "PeerPluginStatus.hpp" // signal parameter
#include "controller/Controller.hpp" // needed to connect
#include "Request/TorrentPluginRequest.hpp"
//#include "Alert/TorrentPluginStatusAlert.hpp"
#include "Alert/TorrentPluginAlert.hpp"

#include "controller/Exceptions/InvalidBitSwaprStateEntryException.hpp"

#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint

#include <QLoggingCategory>

/**
 * TorrentPlugin::Status
 */

TorrentPlugin::Status::Status()
    : _numberOfClassicPeers(0)
    , _numberOfObserverPeers(0)
    , _numberOfSellerPeers(0)
    , _numberOfBuyerPeers(0) {
}

TorrentPlugin::Status::Status(quint32 numberOfClassicPeers,
                       quint32 numberOfObserverPeers,
                       quint32 numberOfSellerPeers,
                       quint32 numberOfBuyerPeers)
    : _numberOfClassicPeers(numberOfClassicPeers)
    , _numberOfObserverPeers(numberOfObserverPeers)
    , _numberOfSellerPeers(numberOfSellerPeers)
    , _numberOfBuyerPeers(numberOfBuyerPeers) {
}

quint32 TorrentPlugin::Status::numberOfClassicPeers() const {
    return _numberOfClassicPeers;
}

void TorrentPlugin::Status::setNumberOfClassicPeers(quint32 numberOfClassicPeers) {
    _numberOfClassicPeers = numberOfClassicPeers;
}

quint32 TorrentPlugin::Status::numberOfObserverPeers() const {
    return _numberOfObserverPeers;
}

void TorrentPlugin::Status::setNumberOfObserverPeers(quint32 numberOfObserverPeers) {
    _numberOfObserverPeers = numberOfObserverPeers;
}

quint32 TorrentPlugin::Status::numberOfSellerPeers() const {
    return _numberOfSellerPeers;
}

void TorrentPlugin::Status::setNumberOfSellerPeers(quint32 numberOfSellerPeers) {
    _numberOfSellerPeers = numberOfSellerPeers;
}

quint32 TorrentPlugin::Status::numberOfBuyerPeers() const {
    return _numberOfBuyerPeers;
}

void TorrentPlugin::Status::setNumberOfBuyerPeers(quint32 numberOfBuyerPeers) {
    _numberOfBuyerPeers = numberOfBuyerPeers;
}

/**
 * TorrentPlugin::Configuration
 */

TorrentPlugin::Configuration::Configuration() {
}

TorrentPlugin::Configuration::Configuration(const Configuration & configuration)
    : _enableBanningSets(configuration.enableBanningSets()){
}

TorrentPlugin::Configuration::Configuration(bool enableBanningSets)
    : _enableBanningSets(enableBanningSets) {
}

TorrentPlugin::Configuration::Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry) {

}

PluginMode TorrentPlugin::Configuration::pluginMode(libtorrent::entry::dictionary_type & dictionaryEntry) {

    if(dictionaryEntry.count("pluginMode") == 1) {

        // Get entry
        libtorrent::entry pluginModeEntry = dictionaryEntry.find("pluginMode")->second;

        // Check that entry is of type entry::string_type
        if(pluginModeEntry.type() == libtorrent::entry::string_t) {

            // Use as entry::string_type
            libtorrent::entry::string_type pluginModeStringEntry = pluginModeEntry.string();

            if(pluginModeStringEntry.compare("Buyer") == 0)
                return PluginMode::Buyer;
            else if(pluginModeStringEntry.compare("Seller") == 0)
                return PluginMode::Seller;
            else if(pluginModeStringEntry.compare("Observer") == 0)
                return PluginMode::Seller;
            else
                throw InvalidBitSwaprStateEntryException(dictionaryEntry, "pluginMode value must be among {Buyer, Seller, Observer}.");

        } else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "pluginMode key is not of type entry::string_type.");

    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "pluginMode key should have .count == 1.");
}

void TorrentPlugin::Configuration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

}
bool TorrentPlugin::Configuration::enableBanningSets() const {
    return _enableBanningSets;
}

void TorrentPlugin::Configuration::setEnableBanningSets(bool enableBanningSets) {
    _enableBanningSets = enableBanningSets;
}

/**
 * TorrentPlugin::TorrentPlugin
 */

TorrentPlugin::TorrentPlugin(Plugin * plugin,
                             const boost::shared_ptr<libtorrent::torrent> & torrent,
                             const TorrentPlugin::Configuration & configuration,
                             QLoggingCategory & category)
    : _plugin(plugin)
    , _torrent(torrent)
    , _enableBanningSets(configuration.enableBanningSets())
    , _category(category) {
}

TorrentPlugin::~TorrentPlugin() {

    // Lets log, so we understand when libtorrent disposes of shared pointer
    qCDebug(_category) << "~TorrentPlugin() called.";
}

/**
boost::shared_ptr<libtorrent::peer_plugin> TorrentPlugin::peerPlugin(const libtorrent::tcp::endpoint & endPoint) {

    // Lookup plugin based on endpoint
    QMap<libtorrent::tcp::endpoint, boost::shared_ptr<libtorrent::peer_plugin> >::const_iterator i = _peerPlugins.find(endPoint);

    // Return weak pointer for match, otherwise throw exception
    if(i != _peerPlugins.end())
        return i.value();
    else
        throw std::exception();
}


 * DO WE EVEN THIS?
void TorrentPlugin::_tick() {

    // Iterate peers, and handle disconnection or new messages
    QList<libtorrent::tcp::endpoint> keys = _peerPlugins.keys();
    for(QList<libtorrent::tcp::endpoint>::iterator i = keys.begin(),
            end(keys.end()); i != end;i++) {

        // Get peer plugin
        PeerPlugin * plugin = _peerPlugins.find(*i);

        // If peer was disconnected, or sent inappropriate message, then remove plugin
        if(!plugin->isConnected() || plugin->peerSentInvalidMessage())
            removePeerPlugin(plugin);
        else // otherwise process messages
            plugin->processUnprocessedMessages();
    }

}
*/

void TorrentPlugin::addToPeersWithoutExtensionSet(const libtorrent::tcp::endpoint & endPoint) {
    _peersWithoutExtension.insert(endPoint);
}

void TorrentPlugin::addToIrregularPeersSet(const libtorrent::tcp::endpoint & endPoint) {
    _irregularPeer.insert(endPoint);
}

Plugin * TorrentPlugin::plugin() {
    return _plugin;
}

boost::shared_ptr<libtorrent::torrent> TorrentPlugin::torrent() const {
    return _torrent;
}

void TorrentPlugin::setTorrent(const boost::shared_ptr<libtorrent::torrent> &torrent) {
    _torrent = torrent;
}

bool TorrentPlugin::isPeerWellBehaved(libtorrent::peer_connection * connection) const {
    
    // Get endpoint of connection
    const libtorrent::tcp::endpoint & endPoint = connection->remote();
    
    // If we are using banning sets, then check this peer
    if(_enableBanningSets) {

        // Check if we know from before that peer does not have
        if(_peersWithoutExtension.contains(endPoint)) {
            qCDebug(_category) << "Peer is known to not have extension.";
            return false;
        }

        // Check if peer is banned due to irregular behaviour
        if(_irregularPeer.contains(endPoint)) {
            qCDebug(_category) << "Peer has been banned due to irregular behaviour.";
            return false;
        }
    }

    // Check that this is indeed a bittorrent client, and not a HTTP or URL seed, if not, then dont isntall
    if(connection->type() != libtorrent::peer_connection::bittorrent_connection) {
        qCDebug(_category) << "Peer was not BitTorrent client.";
        return false;
    }

    // Is well behaved
    return true;
}

/**
void TorrentPlugin::processTorrentPluginRequest(const TorrentPluginRequest * request) {

    qCDebug(_category) << "processTorrentPluginRequest";

    if(const StartPluginTorrentPluginRequest * r = dynamic_cast<const StartPluginTorrentPluginRequest *>(request)) {

        qCDebug(_category) << "StartPluginTorrentPluginRequest";
        processStartPluginRequest(r);
    } else if(const SetConfigurationTorrentPluginRequest * r = dynamic_cast<const SetConfigurationTorrentPluginRequest *>(request)) {

        qCDebug(_category) << "SetConfigurationTorrentPluginRequest";
        processSetConfigurationTorrentPluginRequest(r);
    } else if(const SetPluginModeTorrentPluginRequest * r = dynamic_cast<const SetPluginModeTorrentPluginRequest *>(request)) {

        qCDebug(_category) << "SetConfigurationTorrentPluginRequest";
        //processSetPluginModeTorrentPluginRequest(r);
    }
}
*/

void TorrentPlugin::sendTorrentPluginAlert(const TorrentPluginAlert & alert) {
    _torrent->alerts().post_alert(alert);
}

/*
TorrentPluginConfiguration TorrentPlugin::config() const {
    return _torrentPluginConfiguration;
}
*/

/**
TorrentPluginStatusAlert TorrentPlugin::createTorrentPluginStatusAlert() {


    int numberOfPeers = _peerPlugins.size();

    int numberOfPeersWithExtension = 0;
    for(std::map<libtorrent::tcp::endpoint, PeerPlugin *>::iterator i = _peerPlugins.begin(),
        end(_peerPlugins.end()); i != end; i++) {

        // Count as supporting plugin if extended handshake was successful
        if((i->second)->peerBEP43SupportedStatus() == BEPSupportStatus::supported)
            numberOfPeersWithExtension++;
    }

    // Create torrent plugin alert
    return TorrentPluginStatusAlert(_torrent->info_hash(), numberOfPeers, numberOfPeersWithExtension, _pluginStarted, 0, 0, _torrentPluginConfiguration->pluginMode());

}
*/
