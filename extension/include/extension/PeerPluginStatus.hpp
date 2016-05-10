/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2015
 */

#ifndef JOYSTREAM_EXTENSION_PEER_PLUGIN_STATUS_HPP
#define JOYSTREAM_EXTENSION_PEER_PLUGIN_STATUS_HPP

#include <extension/PeerModeAnnounced.hpp>
#include <extension/PluginMode.hpp>
#include <extension/BEPSupportStatus.hpp>

#include <QtGlobal> // remove later when done with quints

namespace joystream {
namespace extension {

    class PeerPluginStatus {

    public:

        // Defualt constructor
        PeerPluginStatus();

        // Constructor from members
        PeerPluginStatus(PeerModeAnnounced peerModeAnnounced, BEPSupportStatus peerBEP10SupportStatus, BEPSupportStatus peerJoyStreamBEPSupportStatus);

        // Getters and setters
        virtual PluginMode pluginMode() const = 0;

        PeerModeAnnounced peerModeAnnounced() const;
        void setPeerModeAnnounced(PeerModeAnnounced peerModeAnnounced);

        BEPSupportStatus peerBEP10SupportStatus() const;
        void setPeerBEP10SupportStatus(BEPSupportStatus peerBEP10SupportStatus);

        BEPSupportStatus peerJoyStreamBEPSupportStatus() const;
        void setPeerBitSwaprBEPSupportStatus(BEPSupportStatus peerJoyStreamBEPSupportStatus);

    private:

        // Announced peer mode
        PeerModeAnnounced _peerModeAnnounced;

        // Indicates whether peer supports BEP10
        BEPSupportStatus _peerBEP10SupportStatus;

        // Indicates whether peer supports BEP43 .. BitSwapr
        BEPSupportStatus _peerBitSwaprBEPSupportStatus;

        // Net amount of funds (satoshies) sent
        quint64 _balance;
    };
}
}

#endif // JOYSTREAM_EXTENSION_PEER_PLUGIN_STATUS_HPP

