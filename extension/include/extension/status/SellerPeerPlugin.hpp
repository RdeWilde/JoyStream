/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#ifndef EXTENSION_SELLER_PEER_PLUGIN_HPP
#define EXTENSION_SELLER_PEER_PLUGIN_HPP

#include <extension/status/PeerPlugin.hpp>

// *** TEMPORARY INCLUDE TO GET INNER CLASSES ***
#include <extension/SellerPeerPlugin.hpp>
// *** TEMPORARY INCLUDE TO GET INNER CLASSES ***

namespace joystream {
namespace extension {
namespace status {

    /**
     * @brief Plugin status, that is a snapshot of important information.
     */
    class SellerPeerPlugin : public PeerPlugin {

    public:

        // Default constructor
        SellerPeerPlugin();

        // Constructor from members
        SellerPeerPlugin(PeerModeAnnounced peerModeAnnounced,
               BEPSupportStatus peerBEP10SupportStatus,
               BEPSupportStatus peerJoyStreamBEPSupportStatus,
               const PeerState & peerState,
               ClientState clientState,
               const Payee::Status & payeeStatus);

        virtual PluginMode pluginMode() const;

        // Getters and setters
        PeerState peerState() const;
        void setPeerState(const PeerState &peerState);

        ClientState clientState() const;
        void setClientState(ClientState clientState);

        Payee::Status payeeStatus() const;
        void setPayeeStatus(const Payee::Status &payeeStatus);

    private:

        // State of peer
        joystream::extension::SellerPeerPlugin::PeerState _peerState;

        // State of client
        joystream::extension::SellerPeerPlugin::ClientState _clientState;

        // Status of payee
        Payee::Status _payeeStatus;
    };

}
}
}

#endif // EXTENSION_SELLER_PEER_PLUGIN_HPP

