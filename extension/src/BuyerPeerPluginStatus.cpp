/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#include <extension/BuyerPeerPluginStatus.hpp>

namespace joystream {
namespace extension {

    BuyerPeerPluginStatus::BuyerPeerPluginStatus() {
    }

    BuyerPeerPluginStatus::BuyerPeerPluginStatus(PeerModeAnnounced peerModeAnnounced,
                                    BEPSupportStatus peerBEP10SupportStatus,
                                    BEPSupportStatus peerBitSwaprBEPSupportStatus,
                                    const BuyerPeerPluginPeerState & peerState,
                                    BuyerPeerPluginClientState clientState,
                                    quint32 payorSlot,
                                    int indexOfAssignedPiece,
                                    const QList<int> & downloadedValidPieces)
        : PeerPluginStatus(peerModeAnnounced,
                             peerBEP10SupportStatus,
                             peerBitSwaprBEPSupportStatus)
        , _peerState(peerState)
        , _clientState(clientState)
        , _payorSlot(payorSlot)
        , _indexOfAssignedPiece(indexOfAssignedPiece)
        , _downloadedValidPieces(downloadedValidPieces) {
    }

    PluginMode BuyerPeerPluginStatus::pluginMode() const {
        return PluginMode::Buyer;
    }

    BuyerPeerPluginPeerState BuyerPeerPluginStatus::peerState() const {
        return _peerState;
    }

    void BuyerPeerPluginStatus::setPeerState(const BuyerPeerPluginPeerState & peerState) {
        _peerState = peerState;
    }

    BuyerPeerPluginClientState BuyerPeerPluginStatus::clientState() const {
        return _clientState;
    }

    void BuyerPeerPluginStatus::setClientState(BuyerPeerPluginClientState clientState) {
        _clientState = clientState;
    }

    QList<int> BuyerPeerPluginStatus::downloadedValidPieces() const {
        return _downloadedValidPieces;
    }

    void BuyerPeerPluginStatus::setDownloadedValidPieces(const QList<int> & downloadedValidPieces) {
        _downloadedValidPieces = downloadedValidPieces;
    }

    int BuyerPeerPluginStatus::indexOfAssignedPiece() const {
        return _indexOfAssignedPiece;
    }

    void BuyerPeerPluginStatus::setIndexOfAssignedPiece(int indexOfAssignedPiece){
        _indexOfAssignedPiece = indexOfAssignedPiece;
    }

    quint32 BuyerPeerPluginStatus::payorSlot() const {
        return _payorSlot;
    }

    void BuyerPeerPluginStatus::setPayorSlot(quint32 payorSlot) {
        _payorSlot = payorSlot;
    }

}
}
