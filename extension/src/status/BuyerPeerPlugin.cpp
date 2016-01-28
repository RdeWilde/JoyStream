/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#include <extension/status/BuyerPeerPlugin.hpp>

namespace joystream {
namespace extension {

    BuyerPeerPlugin::BuyerPeerPlugin() {
    }

    BuyerPeerPlugin::BuyerPeerPlugin(PeerModeAnnounced peerModeAnnounced,
                                    BEPSupportStatus peerBEP10SupportStatus,
                                    BEPSupportStatus peerBitSwaprBEPSupportStatus,
                                    const PeerState & peerState,
                                    ClientState clientState,
                                    quint32 payorSlot,
                                    int indexOfAssignedPiece,
                                    const QList<int> & downloadedValidPieces)
        : PeerPlugin::Status(peerModeAnnounced,
                             peerBEP10SupportStatus,
                             peerBitSwaprBEPSupportStatus)
        , _peerState(peerState)
        , _clientState(clientState)
        , _payorSlot(payorSlot)
        , _indexOfAssignedPiece(indexOfAssignedPiece)
        , _downloadedValidPieces(downloadedValidPieces) {
    }

    PluginMode BuyerPeerPlugin::pluginMode() const {
        return PluginMode::Buyer;
    }

    BuyerPeerPlugin::PeerState BuyerPeerPlugin::peerState() const {
        return _peerState;
    }

    void BuyerPeerPlugin::setPeerState(const PeerState & peerState) {
        _peerState = peerState;
    }

    BuyerPeerPlugin::ClientState BuyerPeerPlugin::clientState() const {
        return _clientState;
    }

    void BuyerPeerPlugin::setClientState(ClientState clientState) {
        _clientState = clientState;
    }

    QList<int> BuyerPeerPlugin::downloadedValidPieces() const {
        return _downloadedValidPieces;
    }

    void BuyerPeerPlugin::setDownloadedValidPieces(const QList<int> & downloadedValidPieces) {
        _downloadedValidPieces = downloadedValidPieces;
    }

    int BuyerPeerPlugin::indexOfAssignedPiece() const {
        return _indexOfAssignedPiece;
    }

    void BuyerPeerPlugin::setIndexOfAssignedPiece(int indexOfAssignedPiece){
        _indexOfAssignedPiece = indexOfAssignedPiece;
    }

    quint32 BuyerPeerPlugin::payorSlot() const {
        return _payorSlot;
    }

    void BuyerPeerPlugin::setPayorSlot(quint32 payorSlot) {
        _payorSlot = payorSlot;
    }

    quint32 BuyerPeerPlugin::payorSlot() const {
        return _payorSlot;
    }

    void BuyerPeerPlugin::setPayorSlot(quint32 payorSlot) {
        _payorSlot = payorSlot;
    }

}
}
