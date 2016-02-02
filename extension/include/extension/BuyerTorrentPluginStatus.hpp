/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#ifndef JOYSTREAM_EXTENSION_STATUS_BUYER_TORRENT_PLUGIN_HPP
#define JOYSTREAM_EXTENSION_STATUS_BUYER_TORRENT_PLUGIN_HPP

#include <extension/BuyerTorrentPluginState.hpp>
#include <extension/BuyerPeerPluginStatus.hpp>
#include <extension/PayorStatus.hpp>

#include <libtorrent/socket.hpp> // libtorrent::tcp::endpoint
#include <QMap>

namespace joystream {
namespace extension {

    class BuyerTorrentPluginStatus  {

    public:

        BuyerTorrentPluginStatus();

        BuyerTorrentPluginStatus(BuyerTorrentPluginState state,
               const QMap<libtorrent::tcp::endpoint, BuyerPeerPluginStatus> & peerPluginStatuses,
               const PayorStatus & payor);

        // Getters and setters
        BuyerTorrentPluginState state() const;
        void setState(BuyerTorrentPluginState state);

        QMap<libtorrent::tcp::endpoint, BuyerPeerPluginStatus> peerPluginStatuses() const;
        void setPeerPluginStatuses(const QMap<libtorrent::tcp::endpoint, BuyerPeerPluginStatus> & peerPluginStatuses);

        PayorStatus payor() const;
        void setPayor(const PayorStatus & payor);

    private:

        // State of plugin
        // to fix
        BuyerTorrentPluginState _state;

        // Status of peers
        QMap<libtorrent::tcp::endpoint, BuyerPeerPluginStatus> _peerPluginStatuses;

        // Status of the payor
        PayorStatus _payor;
    };

}
}

//#include <QMetaType>
//Q_DECLARE_METATYPE(BuyerTorrentPlugin::State)


#endif // JOYSTREAM_EXTENSION_STATUS_BUYER_TORRENT_PLUGIN_HPP
