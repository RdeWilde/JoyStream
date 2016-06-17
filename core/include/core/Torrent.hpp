/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 17 2016
 */

#ifndef JOYSTREAM_CORE_TORRENT_HPP
#define JOYSTREAM_CORE_TORRENT_HPP

#include <QObject>

namespace joystream {
namespace core {

/** Make sure that there are only const routines and signals on all types **/

/**
class PeerPlugin {

};

class TorrentPlugin {

};
*/

// Torrent
class Torrent : public QObject {

    Q_OBJECT

public:

    // streaming routines
    // this can be used to stream into a http daemon, etc.
    // *initiating stream (register a callback)
    // *stopping stream

    /**
     * Move onto Torrent

    //void startTorrentPlugin(const libtorrent::sha1_hash & info_hash, const TorrentPlugin::Configuration * configuration);
    void startSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const SellerTorrentPlugin::Configuration & pluginConfiguration);
    void startBuyerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const Coin::UnspentP2PKHOutput & utxo);
    //void startObserverTorrentPlugin(const libtorrent::sha1_hash & info_hash, const ObserverTorrentPlugin::Configuration & pluginConfiguration);


    void pauseTorrent(const libtorrent::sha1_hash & info_hash);
    void startTorrent(const libtorrent::sha1_hash & info_hash);

    */

    // A torrent plugin was started
    // *** DISCONTINUED, THESE ARE INSTALLED AS SIGNALS ON TORRENT VIEW MODELS
    //void torrentPluginStarted();
    //void startedSellerTorrentPlugin(SellerTorrentPluginViewModel * model);
    //void startedBuyerTorrentPlugin(BuyerTorrentPluginViewModel * model);

private:

    //TorrentPlugin _torrentPlugin;

};

}
}

#endif // JOYSTREAM_CORE_TORRENT_HPP
