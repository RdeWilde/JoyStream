/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2015
 */

#include <extension/status/PeerPlugin.hpp>

namespace joystream {
namespace extension {
namespace status {

    PeerPlugin::PeerPlugin() {
    }

    PeerPlugin::PeerPlugin(PeerModeAnnounced peerModeAnnounced, BEPSupportStatus peerBEP10SupportStatus, BEPSupportStatus peerBitSwaprBEPSupportStatus)
        : _peerModeAnnounced(peerModeAnnounced)
        , _peerBEP10SupportStatus(peerBEP10SupportStatus)
        , _peerBitSwaprBEPSupportStatus(peerBitSwaprBEPSupportStatus) {
    }

    BEPSupportStatus PeerPlugin::peerJoyStreamBEPSupportStatus() const {
        return _peerBitSwaprBEPSupportStatus;
    }

    void PeerPlugin::setPeerBitSwaprBEPSupportStatus(BEPSupportStatus peerBitSwaprBEPSupportStatus) {
        _peerBitSwaprBEPSupportStatus = peerBitSwaprBEPSupportStatus;
    }

    BEPSupportStatus PeerPlugin::peerBEP10SupportStatus() const {
        return _peerBEP10SupportStatus;
    }

    void PeerPlugin::setPeerBEP10SupportStatus(BEPSupportStatus peerBEP10SupportStatus) {
        _peerBEP10SupportStatus = peerBEP10SupportStatus;
    }

    PeerModeAnnounced PeerPlugin::peerModeAnnounced() const {
        return _peerModeAnnounced;
    }

    void PeerPlugin::setPeerModeAnnounced(PeerModeAnnounced peerModeAnnounced) {
        _peerModeAnnounced = peerModeAnnounced;
    }

}
}
}
