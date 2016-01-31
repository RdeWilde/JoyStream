/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#ifndef JOYSTREAM_EXTENSION_STATUS_BUYER_PEER_PLUGIN_HPP
#define JOYSTREAM_EXTENSION_STATUS_BUYER_PEER_PLUGIN_HPP

#include <extension/status/PeerPlugin.hpp>

// *** TEMPORARY INCLUDE TO GET INNER CLASSES ***
#include <extension/BuyerPeerPlugin.hpp>
// *** TEMPORARY INCLUDE TO GET INNER CLASSES ***

#include <QList>

namespace joystream {
namespace extension {
namespace status {

    class BuyerPeerPlugin : public PeerPlugin {

    public:

        // Default constructor
        BuyerPeerPlugin();

        // Constructor from members
        BuyerPeerPlugin(PeerModeAnnounced peerModeAnnounced,
               BEPSupportStatus peerBEP10SupportStatus,
               BEPSupportStatus peerJoyStreamBEPSupportStatus,
               const PeerState & peerState,
               ClientState clientState,
               quint32 payorSlot,
               int indexOfAssignedPiece,
               const QList<int> & downloadedValidPieces);

        // Getters and setters
        virtual PluginMode pluginMode() const;

        PeerState peerState() const;
        void setPeerState(const PeerState & peerState);

        ClientState clientState() const;
        void setClientState(ClientState clientState);

        quint32 payorSlot() const;
        void setPayorSlot(const quint32 payorSlot);

        int indexOfAssignedPiece() const;
        void setIndexOfAssignedPiece(int indexOfAssignedPiece);

        QList<int> downloadedValidPieces() const;
        void setDownloadedValidPieces(const QList<int> & downloadedValidPieces);

    private:

        // State of peer
        // to fix...
        BuyerPeerPlugin::PeerState _peerState;

        // State of client
        // to fix...
        BuyerPeerPlugin::ClientState _clientState;

        // Peer plugin position in Payor
        quint32 _payorSlot;

        // Index of a piece assigned to this peer
        int _indexOfAssignedPiece;

        // Piece indexes, in download order, of
        // all valid pieces downloaded from seller peer during this session
        QList<int> _downloadedValidPieces;
    };

}
}
}


#endif // JOYSTREAM_EXTENSION_STATUS_BUYER_PEER_PLUGIN_HPP
