#include "BuyerPeerPlugin.hpp"

#include <QLoggingCategory> // qCDebug

BuyerPeerPlugin::BuyerPeerPlugin(TorrentPlugin * torrentPlugin,
                                 libtorrent::bt_peer_connection * bittorrentPeerConnection,
                                 QLoggingCategory & category)
    : PeerPlugin(torrentPlugin,bittorrentPeerConnection,category) {
}

/*
 * Called when an extended message is received. If returning true,
 * the message is not processed by any other plugin and if false
 * is returned the next plugin in the chain will receive it to
 * be able to handle it this is not called for web seeds.
 */
bool BuyerPeerPlugin::on_extended(int length, int msg, libtorrent::buffer::const_interval body) {

    /**
    // No processing is done before a successful extended handshake
    if(_peerBitSwaprBEPSupportedStatus != BEPSupportStatus::supported)
        return;
    */

    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "buyer:on_extended(" << length << "," << msg << ")";

    // Update peerPluginState_

    // Create QByte array where you put data, then wrap witha stream, after mesage has b een parsed, check that pointer received is not null.

    // CRITICAL
    return false;
}

/*
 * Called aproximately once every second
 */
void BuyerPeerPlugin::tick() {

    // call parent tick() also?

    qCDebug(category_) << "BuyerPeerPlugin.tick()";
}

void BuyerPeerPlugin::sendStatusToController() {

    // Send status to controller
    //PeerPluginStatus status(_peerPluginConfiguration.getPeerPluginId(), _peerPluginConfiguration.getPeerPluginState(), 0);
    PeerPluginStatus status(_peerPluginId, _lastPeerAction, 0);

    //emit peerPluginStatusUpdated(status);
}
