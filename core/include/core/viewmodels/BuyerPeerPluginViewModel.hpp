/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_CORE_VIEWMODELS_BUYER_PEER_PLUGIN_VIEW_MODEL_HPP
#define JOYSTREAM_CORE_VIEWMODELS_BUYER_PEER_PLUGIN_VIEW_MODEL_HPP

#include <extension/BuyerPeerPlugin.hpp> // BuyerPeerPlugin::Status

namespace joystream {
namespace core {
namespace viewmodels {

    class BuyerPeerPluginViewModel : public QObject
    {
        Q_OBJECT

    public:

        // Constructor
        BuyerPeerPluginViewModel(QObject * parent,
                                 const libtorrent::tcp::endpoint & endPoint,
                                 const BuyerPeerPlugin::Status & status);

        // Update status
        void update(const BuyerPeerPlugin::Status & status);

        // Getters
        libtorrent::tcp::endpoint endPoint() const;
        BuyerPeerPlugin::Status status() const;

    signals:

        // Status change
        void clientStateChanged(BuyerPeerPlugin::ClientState state);
        void payorSlotChanged(quint32 payorSlot);

    private:

        // Peer endpoint
        libtorrent::tcp::endpoint _endPoint;

        // Status of peer plugin
        BuyerPeerPlugin::Status _status;
    };

}
}
}

#endif // JOYSTREAM_CORE_VIEWMODELS_BUYER_PEER_PLUGIN_VIEW_MODEL_HPP
