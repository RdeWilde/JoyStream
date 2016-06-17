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

    /// Plugin operations

    void toSell(const SellerTorrentPlugin::Configuration & pluginConfiguration);
    void toBuy(const BuyerTorrentPlugin::Configuration & pluginConfiguration, const Coin::UnspentP2PKHOutput & utxo);
    void toObserve(const ObserverTorrentPlugin::Configuration & pluginConfiguration);

    void start();
    void pause();
    void stop();

    */

signals:

private:

    //TorrentPlugin _torrentPlugin;

};

}
}

#endif // JOYSTREAM_CORE_TORRENT_HPP
