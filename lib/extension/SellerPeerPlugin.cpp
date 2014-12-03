#include "SellerPeerPlugin.hpp"

#include <QLoggingCategory> // qCDebug

SellerPeerPlugin::SellerPeerPlugin(TorrentPlugin * torrentPlugin,
                                   libtorrent::bt_peer_connection * bittorrentPeerConnection,
                                   QLoggingCategory & category)
      : PeerPlugin(torrentPlugin, bittorrentPeerConnection, category)
{

}

/*
 * Called when an extended message is received. If returning true,
 * the message is not processed by any other plugin and if false
 * is returned the next plugin in the chain will receive it to
 * be able to handle it this is not called for web seeds.
 */
bool SellerPeerPlugin::on_extended(int length, int msg, libtorrent::buffer::const_interval body) {

    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "seller:on_extended(" << length << "," << msg << ")";

    // Update peerPluginState_

    // Create QByte array where you put data, then wrap witha stream, after mesage has b een parsed, check that pointer received is not null.

    // CRITICAL
    return false;
}

/*
 * Called aproximately once every second
 */
void SellerPeerPlugin::tick() {

    // call parent tick() also?

    qCDebug(category_) << "SellerPeerPlugin.tick()";
}
