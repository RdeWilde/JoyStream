#include "BuyerTorrentPlugin.hpp"

/**
 * BuyerTorrentPlugin::Status
 */

BuyerTorrentPlugin::Status::Status(State state,
                                   const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peers,
                                   const Payor::Status & payor)
    : _state(state)
    , _peers(peers)
    , _payor(payor) {
}

BuyerTorrentPlugin::State BuyerTorrentPlugin::Status::state() const {
    return _state;
}

void BuyerTorrentPlugin::Status::setState(State state) {
    _state = state;
}

/**
 * BuyerTorrentPlugin::Configuration
 */

//#include "BitCoin/BitSwaprjs.hpp"

#include <QLoggingCategory>

#include <libtorrent/entry.hpp>

BuyerTorrentPlugin::Configuration::Configuration(const Configuration & c)
    : TorrentPlugin::Configuration(c)
    , _state(c.state())
    //, _peerConfigurations(c.peerConfigurations())
    , _payorConfiguration(c.payorConfiguration()) {
}

BuyerTorrentPlugin::Configuration::Configuration(bool enableBanningSets,
                                                    State state,
                                                    //const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Configuration> & peers,
                                                    const Payor::Configuration & payor)
    : TorrentPlugin::Configuration(enableBanningSets)
    , _state(state)
    //, _peerConfigurations(peers)
    , _payorConfiguration(payor){
}

/**
BuyerTorrentPlugin::Configuration::Configuration(QVector<quint64> funds,
                                                quint64 changeValue,
                                                const OutPoint & fundingOutput,
                                                const KeyPair & fundingOutputKeyPair,
                                                quint64 maxPrice,
                                                quint32 maxLock)
    : TorrentPlugin::Configuration(true)
    , _state(State::waiting_for_payor_to_be_ready)
    , _payorConfiguration(funds, changeValue, fundingOutput, fundingOutputKeyPair, maxPrice, maxLock) {
}
*/

BuyerTorrentPlugin::Configuration::Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry)
    : TorrentPlugin::Configuration(dictionaryEntry) {
    // IMPLEMENT LATER
}

void BuyerTorrentPlugin::Configuration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

    // Call super version
    //TorrentPlugin::Configuration::toDictionaryEntry(dictionaryEntry);

    // IMPLEMENT LATER
}

PluginMode BuyerTorrentPlugin::Configuration::pluginMode() const {
    return PluginMode::Buyer;
}

BuyerTorrentPlugin::State BuyerTorrentPlugin::Configuration::state() const {
    return _state;
}

Payor::Configuration BuyerTorrentPlugin::Configuration::payorConfiguration() const {
    return _payorConfiguration;
}

void BuyerTorrentPlugin::Configuration::setPayorConfiguration(const Payor::Configuration &payorConfiguration) {
    _payorConfiguration = payorConfiguration;
}

/**
QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Configuration> BuyerTorrentPlugin::Configuration::peerConfigurations() const {
    return _peerConfigurations;
}

void BuyerTorrentPlugin::Configuration::setPeerConfigurations(const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Configuration> &peerConfigurations) {
    _peerConfigurations = peerConfigurations;
}
*/

/**
 * BuyerTorrentPlugin
 */

#include "BuyerPeerPlugin.hpp"

#include "Message/Buy.hpp"
#include "Message/Sell.hpp"
#include "Message/Observe.hpp"
#include "Message/JoinContract.hpp"
#include "Message/SignRefund.hpp"

#include "Alert/BuyerTorrentPluginStatuAlert.hpp"

#include "BitCoin/Wallet.hpp"

#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint

// Maximum allowable a peer may have in responding to given message (ms)
#define SIGN_REFUND_MAX_DELAY 5*1000

BuyerTorrentPlugin::BuyerTorrentPlugin(Plugin * plugin,
                                       const boost::weak_ptr<libtorrent::torrent> & torrent,
                                       Wallet * wallet,
                                       const BuyerTorrentPlugin::Configuration & configuration,
                                       QLoggingCategory & category)
    : TorrentPlugin(plugin, torrent, configuration, category)
    , _slotToPluginMapping(configuration.payorConfiguration().channels().size(), NULL)
    , _wallet(wallet)
    , _payor(_wallet, configuration.payorConfiguration()) {
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
    BuyerPeerPlugin::Configuration configuration(ExtendedMessageIdMapping(),
                                                   ExtendedMessageIdMapping(),
                                                   BEPSupportStatus::unknown,
                                                   BEPSupportStatus::unknown,
                                                   BuyerPeerPlugin::PeerState(),
                                                   BuyerPeerPlugin::ClientState::no_bitswapr_message_sent,
                                                   0);

    boost::shared_ptr<BuyerPeerPlugin> sharedPeerPluginPtr(new BuyerPeerPlugin(this,
                                                                           btConnection,
                                                                           configuration,
                                                                           _category));
    // Add to collection
    _peers[endPoint] = boost::weak_ptr<BuyerPeerPlugin>(sharedPeerPluginPtr);

    qCDebug(_category) << "Buyer #" << _peers.count() << endPointString.c_str() << "added.";

    // Return pointer to plugin as required
    return sharedPeerPluginPtr;
}

void BuyerTorrentPlugin::on_piece_pass(int index) {

}

void BuyerTorrentPlugin::on_piece_failed(int index) {

}

void BuyerTorrentPlugin::tick() {

    qCDebug(_category) << "BuyerTorrentPlugin.tick()";

    // Send status update to controller
    sendTorrentPluginAlert(BuyerTorrentPluginStatusAlert(_infoHash, status()));
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

bool BuyerTorrentPlugin::inviteSeller(quint32 minPrice, quint32 minLock) const {

    // Check that we are
    // 1) Still building payment channel
    // 2) Price is low enough
    // 3) nLockTime is short enough
    return _state == State::waiting_for_payor_to_be_ready &&
            minPrice <= _payor.maxPrice() &&
            minLock <= _payor.maxLock();
}

quint32 BuyerTorrentPlugin::addSellerToContract(BuyerPeerPlugin * peer, quint64 price,const PublicKey & contractPk, const PublicKey & finalPk, quint32 refundLockTime) {

    // Check payor is trying to find sellers
    if(_payor.state() != Payor::State::waiting_for_full_set_of_sellers)
        throw std::exception("Contract cannot accept new sellers at present.");

    // _payor.state() == Payor::State::waiting_for_full_set_of_sellers =>
    Q_ASSERT(_state == State::waiting_for_payor_to_be_ready);

    // Try to assign slot in payment channel
    quint32 slot = _payor.assignUnassignedSlot(price, contractPk, finalPk, refundLockTime);

    // Unassigned slots should always have null pointer
    Q_ASSERT(_slotToPluginMapping[slot] == NULL);

    // Save reference to peer for given slot
    _slotToPluginMapping[slot] = peer;

    // If this was the last slot, then we have a full set,
    // start asking for signatures
    if(isContractFull()) {

        qDebug(_category) << "Contract is full, sending sign_refund messages to sellers.";

        /**
         * For each contract channel,
         * grab value and payor final pk,
         * and send sign_refund message
         */

        // Get channels
        const QVector<Payor::Channel> & channels = _payor.channels();

        // Iterate
        quint32 index = 0;
        const TxId & contractHash = _payor.contractHash();
        for(QVector<Payor::Channel>::const_iterator i = channels.constBegin(),
            end(channels.constEnd(); i != end;i++, index++) {

            // Get channel
            const Payor::Channel & channel = *i;

            // isContractFull() =>
            Q_ASSERT(channel.state() == Payor::Channel::State::assigned);

            // Create sign_refund message
            SignRefund m(contractHash, index, channel.funds(), channel.payorFinalKeyPair().pk());

            // Get corresponding buyer peer plugin
            BuyerPeerPlugin * peer = _slotToPluginMapping[index];

            // channel.state() == Payor::Channel::State::assigned
            // and
            // sign_refund message not yet sent
            // =>
            Q_ASSERT(peer->clientState() == BuyerPeerPlugin::ClientState::invited_to_contract);
            Q_ASSERT(peer->peerState().lastAction() == BuyerPeerPlugin::PeerState::LastValidAction::joined_contract);
            Q_ASSERT(peer->peerState().failureMode() == BuyerPeerPlugin::PeerState::FailureMode::not_failed);

            // Have plugin send message
            peer->sendExtendedMessage(m);

            // Update peer plugin client state
            peer->setClientState(BuyerPeerPlugin::ClientState::asked_for_refund_signature);
        }

        // Update payor state
        _payor.setState(Payor::State::waiting_for_full_set_of_refund_signatures);
    }

    return slot;
}

bool BuyerTorrentPlugin::isContractFull() const {
    return _payor.numberOfChannelsWithState(Payor::Channel::State::unassigned) == 0;
}

BuyerTorrentPlugin::Status BuyerTorrentPlugin::status() const {

    // Build list of buyer peer statuses
    QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> peers;

    for(QMap<libtorrent::tcp::endpoint, boost::weak_ptr<BuyerPeerPlugin> >::const_iterator i = _peers.constBegin(),
            end(_peers.constEnd());i != end;i++) {

        // Try to get shared pointer, and get status for peer plugin
        if(boost::shared_ptr<BuyerPeerPlugin> sharedPtr = i.value().lock())
            peers[i.key()] = sharedPtr->status();
        else
            qCDebug(_category) << "BuyerPeerPlugin was invalid!";
    }

    // Return final status
    return Status(_state, peers, _payor.status());
}

/**
boost::weak_ptr<libtorrent::peer_plugin> BuyerTorrentPlugin::peerPlugin(const libtorrent::tcp::endpoint & endPoint) const {

    if(_peers.contains(endPoint)) {
        return _peers[endPoint];
    else
        return boost::weak_ptr<libtorrent::peer_plugin>(NULL);

}

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

quint64 BuyerTorrentPlugin::maxPrice() const {
    return _payor.maxPrice();
}

quint32 BuyerTorrentPlugin::maxLock() const {
    return _payor.maxLock();
}

/*
const Payor & BuyerTorrentPlugin::payor() const {
    return _payor;
}
*/

PluginMode BuyerTorrentPlugin::pluginMode() const {
    return PluginMode::Buyer;
}

