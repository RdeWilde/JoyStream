/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#ifndef EXTENSION_SELLER_PEER_PLUGIN_STATUS_HPP
#define EXTENSION_SELLER_PEER_PLUGIN_STATUS_HPP

#include <extension/PeerPluginStatus.hpp>
#include <extension/SellerPeerPluginPeerState.hpp>
#include <extension/SellerPeerPluginClientState.hpp>
#include <extension/PayeeStatus.hpp>

namespace joystream {
namespace extension {

    class SellerPeerPluginStatus : public PeerPluginStatus {

    public:

        // Default constructor
        SellerPeerPluginStatus();

        // Constructor from members
        SellerPeerPluginStatus(PeerModeAnnounced peerModeAnnounced,
               BEPSupportStatus peerBEP10SupportStatus,
               BEPSupportStatus peerJoyStreamBEPSupportStatus,
               const SellerPeerPluginPeerState & peerState,
               SellerPeerPluginClientState clientState,
               const PayeeStatus & payeeStatus);

        virtual PluginMode pluginMode() const;

        // Getters and setters
        SellerPeerPluginPeerState peerState() const;
        void setPeerState(const SellerPeerPluginPeerState & peerState);

        SellerPeerPluginClientState clientState() const;
        void setClientState(SellerPeerPluginClientState clientState);

        PayeeStatus payeeStatus() const;
        void setPayeeStatus(const PayeeStatus & payeeStatus);

    private:

        // State of peer
        SellerPeerPluginPeerState _peerState;

        // State of client
        SellerPeerPluginClientState _clientState;

        // Status of payee
        PayeeStatus _payeeStatus;
    };

}
}

#endif // EXTENSION_SELLER_PEER_PLUGIN_STATUS_HPP

