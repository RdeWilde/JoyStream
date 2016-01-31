/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2015
 */

#include <extension/PeerPluginStatus.hpp>

namespace joystream {
namespace extension {

    PeerPluginStatus::PeerPluginStatus() {
    }

    PeerPluginStatus::PeerPluginStatus(PeerModeAnnounced peerModeAnnounced, BEPSupportStatus peerBEP10SupportStatus, BEPSupportStatus peerBitSwaprBEPSupportStatus)
        : _peerModeAnnounced(peerModeAnnounced)
        , _peerBEP10SupportStatus(peerBEP10SupportStatus)
        , _peerBitSwaprBEPSupportStatus(peerBitSwaprBEPSupportStatus) {
    }

    BEPSupportStatus PeerPluginStatus::peerJoyStreamBEPSupportStatus() const {
        return _peerBitSwaprBEPSupportStatus;
    }

    void PeerPluginStatus::setPeerBitSwaprBEPSupportStatus(BEPSupportStatus peerBitSwaprBEPSupportStatus) {
        _peerBitSwaprBEPSupportStatus = peerBitSwaprBEPSupportStatus;
    }

    BEPSupportStatus PeerPluginStatus::peerBEP10SupportStatus() const {
        return _peerBEP10SupportStatus;
    }

    void PeerPluginStatus::setPeerBEP10SupportStatus(BEPSupportStatus peerBEP10SupportStatus) {
        _peerBEP10SupportStatus = peerBEP10SupportStatus;
    }

    PeerModeAnnounced PeerPluginStatus::peerModeAnnounced() const {
        return _peerModeAnnounced;
    }

    void PeerPluginStatus::setPeerModeAnnounced(PeerModeAnnounced peerModeAnnounced) {
        _peerModeAnnounced = peerModeAnnounced;
    }

}
}
