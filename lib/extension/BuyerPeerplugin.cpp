#include "BuyerPeerPlugin.hpp"

#include <QLoggingCategory> // qCDebug

BuyerPeerPlugin::BuyerPeerPlugin(TorrentPlugin * torrentPlugin,
                                 libtorrent::bt_peer_connection * bittorrentPeerConnection,
                                 QLoggingCategory & category)
    : PeerPlugin(torrentPlugin,bittorrentPeerConnection,category)
{

}

/*
 * Called when an extended message is received. If returning true,
 * the message is not processed by any other plugin and if false
 * is returned the next plugin in the chain will receive it to
 * be able to handle it this is not called for web seeds.
 */
bool BuyerPeerPlugin::on_extended(int length, int msg, libtorrent::buffer::const_interval body) {

    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "buyer:on_extended(" << length << "," << msg << ")";

    // Create QByte array where you put data, then wrap witha stream, after mesage has b een parsed, check that pointer received is not null.

    // CRITICAL
    return false;
}
