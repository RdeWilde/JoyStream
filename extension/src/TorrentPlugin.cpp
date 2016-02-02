/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/TorrentPlugin.hpp>
#include <extension/Plugin.hpp>
#include <extension/TorrentPluginConfiguration.hpp>
#include <extension/alert/TorrentPluginAlert.hpp>
//#include <controller/exceptions/InvalidBitSwaprStateEntryException.hpp>

#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint

#include <QLoggingCategory>

namespace joystream {
namespace extension {

    TorrentPlugin::TorrentPlugin(Plugin * plugin,
                                 const boost::shared_ptr<libtorrent::torrent> & torrent,
                                 const std::string & bep10ClientIdentifier,
                                 const TorrentPluginConfiguration & configuration,
                                 QLoggingCategory & category)
        : _plugin(plugin)
        , _torrent(torrent)
        , _bep10ClientIdentifier(bep10ClientIdentifier)
        , _enableBanningSets(configuration.enableBanningSets())
        , _category(category) {
    }

    TorrentPlugin::~TorrentPlugin() {

        // Lets log, so we understand when libtorrent disposes of shared pointer
        qCDebug(_category) << "~TorrentPlugin() called.";
    }

    void TorrentPlugin::addToPeersWithoutExtensionSet(const libtorrent::tcp::endpoint & endPoint) {
        _peersWithoutExtension.insert(endPoint);
    }

    void TorrentPlugin::addToIrregularPeersSet(const libtorrent::tcp::endpoint & endPoint) {
        _irregularPeer.insert(endPoint);
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

    void TorrentPlugin::sendTorrentPluginAlert(const alert::TorrentPluginAlert & alert) {
        _torrent->alerts().post_alert(alert);
        //_torrent->alerts().emplace_alert(alert);
    }

    bool TorrentPlugin::enableBanningSets() const
    {
        return _enableBanningSets;
    }

    void TorrentPlugin::setEnableBanningSets(bool enableBanningSets)
    {
        _enableBanningSets = enableBanningSets;
    }

}
}
