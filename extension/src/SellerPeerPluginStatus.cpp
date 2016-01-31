/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#include <extension/SellerPeerPluginStatus.hpp>
#include <extension/PluginMode.hpp>

namespace joystream {
namespace extension {

    SellerPeerPluginStatus::SellerPeerPluginStatus()
        : _clientState(SellerPeerPluginClientState::no_bitswapr_message_sent) {
    }

    SellerPeerPluginStatus::SellerPeerPluginStatus(PeerModeAnnounced peerModeAnnounced,
                                     BEPSupportStatus peerBEP10SupportStatus,
                                     BEPSupportStatus peerBitSwaprBEPSupportStatus,
                                     const SellerPeerPluginPeerState & peerState,
                                     SellerPeerPluginClientState clientState,
                                     const PayeeStatus & payeeStatus)
        : PeerPluginStatus(peerModeAnnounced,
                             peerBEP10SupportStatus,
                             peerBitSwaprBEPSupportStatus)
        , _peerState(peerState)
        , _clientState(clientState)
        , _payeeStatus(payeeStatus) {
    }

    PluginMode SellerPeerPluginStatus::pluginMode() const {
        return PluginMode::Seller;
    }

    SellerPeerPluginPeerState SellerPeerPluginStatus::peerState() const {
        return _peerState;
    }

    void SellerPeerPluginStatus::setPeerState(const SellerPeerPluginPeerState & peerState) {
        _peerState = peerState;
    }

    SellerPeerPluginClientState SellerPeerPluginStatus::clientState() const {
        return _clientState;
    }

    void SellerPeerPluginStatus::setClientState(SellerPeerPluginClientState clientState) {
        _clientState = clientState;
    }

    PayeeStatus SellerPeerPluginStatus::payeeStatus() const {
        return _payeeStatus;
    }

    void SellerPeerPluginStatus::setPayeeStatus(const PayeeStatus & payeeStatus) {
        _payeeStatus = payeeStatus;
    }

}
}

