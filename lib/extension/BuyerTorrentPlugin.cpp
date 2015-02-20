#include "BuyerTorrentPlugin.hpp"
#include "PluginMode.hpp"

#include <QLoggingCategory>

#include <libtorrent/entry.hpp>

BuyerTorrentPlugin::Configuration::Configuration(const Configuration & c)
    : _state(c.state())
    , _maxPrice(c.maxPrice())
    , _maxLock(c.maxLock())
    , _maxFeePerByte(c.maxFeePerByte())
    , _numSellers(c.numSellers()) {
}

BuyerTorrentPlugin::Configuration::Configuration(State stage, quint64 maxPrice, quint32 maxLock, quint64 maxFeePerByte, qint32 numSellers)
    : _state(stage)
    , _maxPrice(maxPrice)
    , _maxLock(maxLock)
    , _maxFeePerByte(maxFeePerByte)
    , _numSellers(numSellers) {
}

BuyerTorrentPlugin::Configuration::Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry)
    : TorrentPlugin::Configuration(dictionaryEntry) {
    // IMPLEMENT LATER
}

void BuyerTorrentPlugin::Configuration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

    // Call super version
    TorrentPlugin::Configuration::toDictionaryEntry(dictionaryEntry);

    // IMPLEMENT LATER
}

PluginMode BuyerTorrentPlugin::Configuration::pluginMode() const {
    return PluginMode::Buyer;
}

BuyerTorrentPlugin::State BuyerTorrentPlugin::Configuration::state() const {
    return _state;
}

void BuyerTorrentPlugin::Configuration::setState(const State & state) {
    _state = state;
}

quint64 BuyerTorrentPlugin::Configuration::maxPrice() const {
    return _maxPrice;
}

void BuyerTorrentPlugin::Configuration::setMaxPrice(quint64 maxPrice) {
    _maxPrice = maxPrice;
}

quint32 BuyerTorrentPlugin::Configuration::maxLock() const {
    return _maxLock;
}

void BuyerTorrentPlugin::Configuration::setMaxLock(quint32 maxLock) {
    _maxLock = maxLock;
}

quint64 BuyerTorrentPlugin::Configuration::maxFeePerByte() const {
    return _maxFeePerByte;
}

void BuyerTorrentPlugin::Configuration::setMaxFeePerByte(quint64 maxFeePerByte) {
    _maxFeePerByte = maxFeePerByte;
}

quint32 BuyerTorrentPlugin::Configuration::numSellers() const {
    return _numSellers;
}

void BuyerTorrentPlugin::Configuration::setNumSellers(quint32 numSellers) {
    _numSellers = numSellers;
}

#include "BuyerPeerPlugin.hpp"

#include "Message/Buy.hpp"
#include "Message/Sell.hpp"
#include "Message/Observe.hpp"
#include "Message/JoinContract.hpp"
#include "Message/SignRefund.hpp"

#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint

// Maximum allowable a peer may have in responding to given message (ms)
#define SIGN_REFUND_MAX_DELAY 5*1000

BuyerTorrentPlugin::BuyerTorrentPlugin(Plugin * plugin,
                                       const boost::weak_ptr<libtorrent::torrent> & torrent,
                                       const BuyerTorrentPlugin::Configuration & configuration,
                                       QLoggingCategory & category)
    : TorrentPlugin(plugin, torrent, configuration, category) {
    //, _configuration(configuration) {

    // do something with this => configuration

    // Start clock for when picking sellers can begin
    _timeSincePluginStarted.start();

    // Setup space for plugins in contract
    //_sellersInContract.fill(NULL, _torrentPluginConfiguration->_numSellers);
}

boost::shared_ptr<libtorrent::peer_plugin> BuyerTorrentPlugin::new_connection(libtorrent::peer_connection * connection) {

    /**
     * Libtorrent docs (http://libtorrent.org/reference-Plugins.html#peer_plugin):
     * The peer_connection will be valid as long as the shared_ptr is being held by the
     * torrent object. So, it is generally a good idea to not keep a shared_ptr to
     * your own peer_plugin. If you want to keep references to it, use weak_ptr.
     */

    // Get end point to look up sets
    const libtorrent::tcp::endpoint & endPoint = connection->remote();
    std::string endPointString = libtorrent::print_endpoint(endPoint);

    qCDebug(_category) << "New connection from" << endPointString.c_str();

    // Check if this peer should be accepted, if not
    // a null is returned, hence plugin is not installed
    if(!TorrentPlugin::isPeerWellBehaved(connection)) {
        qCDebug(_category) << "Rejected connection from peer, peer plugin not installed.";
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }

    // Create bittorrent peer connection
    libtorrent::bt_peer_connection * btConnection = static_cast<libtorrent::bt_peer_connection*>(connection);

    // Create seller buyer peer plugin
    BuyerPeerPlugin::PeerState peerState;
    peerState.setLastAction(BuyerPeerPlugin::PeerState::LastValidAction::no_bitswapr_message_sent);
    peerState.setFailureMode(BuyerPeerPlugin::PeerState::FailureMode::not_failed);

    boost::shared_ptr<libtorrent::peer_plugin> sharedPluginPtr(new BuyerPeerPlugin(this,
                                                                                   btConnection,
                                                                                   peerState,
                                                                                   BuyerPeerPlugin::ClientState
                                                                                   _category));




    // Add to collection
    _peerPlugins[endPoint] = boost::weak_ptr<libtorrent::peer_plugin>(sharedPluginPtr);

    qCDebug(_category) << "Buyer #" << _peerPlugins.count() << endPointString.c_str() << "added.";

    // Return pointer to plugin as required
    return sharedPluginPtr;
}

void BuyerTorrentPlugin::tick() {

    qCDebug(_category) << "BuyerTorrentPlugin.tick()";

    // Call base tick routine
    //TorrentPlugin::_tick();

    /**
Iterate peers and delete the ones which pass this test:

      _lastReceivedMessageWasMalformed || _lastMessageWasStateIncompatible || !_connectionAlive


      */
}

bool BuyerTorrentPlugin::on_resume() {
    return false;
}

bool BuyerTorrentPlugin::on_pause() {
    return false;
}

void BuyerTorrentPlugin::on_files_checked() {

}

void BuyerTorrentPlugin::on_state(int s) {

}

void BuyerTorrentPlugin::on_add_peer(const libtorrent::tcp::endpoint & endPoint, int src, int flags) {

}

/*
void BuyerTorrentPlugin::removePeerPlugin(PeerPlugin * plugin) {

    qCDebug(_category) << "TorrentPlugin::removePeerPlugin(): NOT IMPLEMENTED.";


     * SHOULD DEPEND ON MODE, AND ON SUB MODE STATE

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

}
*/

BuyerTorrentPlugin::State BuyerTorrentPlugin::state() const {
    return _state;
}

void BuyerTorrentPlugin::setState(const State & state) {
    _state = state;
}

PluginMode BuyerTorrentPlugin::pluginMode() const {
    return PluginMode::Buyer;
}

quint64 BuyerTorrentPlugin::maxPrice() const {
    return _maxPrice;
}

void BuyerTorrentPlugin::setMaxPrice(const quint64 &maxPrice) {
    _maxPrice = maxPrice;
}

quint32 BuyerTorrentPlugin::maxLock() const {
    return _maxLock;
}

void BuyerTorrentPlugin::setMaxLock(quint32 maxLock) {
    _maxLock = maxLock;
}

quint64 BuyerTorrentPlugin::maxFeePerByte() const {
    return _maxFeePerByte;
}

void BuyerTorrentPlugin::setMaxFeePerByte(quint64 maxFeePerByte) {
    _maxFeePerByte = maxFeePerByte;
}

quint32 BuyerTorrentPlugin::numSellers() const {
    return _numSellers;
}

void BuyerTorrentPlugin::setNumSellers(quint32 numSellers) {
    _numSellers = numSellers;
}
