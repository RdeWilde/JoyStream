/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#ifndef JOYSTREAM_EXTENSION_STATUS_BUYER_TORRENT_PLUGIN_HPP
#define JOYSTREAM_EXTENSION_STATUS_BUYER_TORRENT_PLUGIN_HPP

// *** TEMPORARY INCLUDE TO GET INNER CLASSES ***
#include <extension/BuyerTorrentPlugin.hpp>
// *** TEMPORARY INCLUDE TO GET INNER CLASSES ***

#include <extension/status/BuyerPeerPlugin.hpp>

namespace joystream {
namespace extension {
namespace status {

    /**
     * @brief Plugin status, that is a snapshot
     * of important information.
     */
    class BuyerTorrentPlugin {

    public:

        BuyerTorrentPlugin();

        BuyerTorrentPlugin(BuyerTorrentPlugin::State state,
               const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin> & peerPluginStatuses,
               const Payor::Status & payor);

        // Getters and setters
        BuyerTorrentPlugin::State state() const;
        void setState(BuyerTorrentPlugin::State state);

        QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin> peerPluginStatuses() const;
        void setPeerPluginStatuses(const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin> & peerPluginStatuses);

        Payor::Status payor() const;
        void setPayor(const Payor::Status & payor);

    private:

        // State of plugin
        // to fix
        BuyerTorrentPlugin::State _state;

        // Status of peers
        QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin> _peerPluginStatuses;

        // Status of the payor
        Payor::Status _payor;
    };


    //#include <QMetaType>
    //Q_DECLARE_METATYPE(BuyerTorrentPlugin::State)


}
}
}

#endif // JOYSTREAM_EXTENSION_STATUS_BUYER_TORRENT_PLUGIN_HPP
