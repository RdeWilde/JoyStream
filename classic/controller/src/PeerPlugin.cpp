/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#include <controller/PeerPlugin.hpp>
#include <gui/gui.hpp>
#include <core/core.hpp>
#include <extension/extension.hpp>

namespace joystream {
namespace classic {
namespace controller {

PeerPlugin::PeerPlugin(core::PeerPlugin * peerPlugin,
                       gui::PeerTreeViewRow * peerTreeViewRow)
    : _peerPlugin(peerPlugin)
    , _peerTreeViewRow(peerTreeViewRow) {

    QObject::connect(peerPlugin,
                     &core::PeerPlugin::peerBEP10SupportStatusChanged,
                     this,
                     &PeerPlugin::setPeerBEP10SupportStatus);

    QObject::connect(peerPlugin,
                     &core::PeerPlugin::peerBitSwaprBEPSupportStatusChanged,
                     this,
                     &PeerPlugin::setPeerBitSwaprBEPSupportStatus);
}

void PeerPlugin::setPeerTreeViewRow(gui::PeerTreeViewRow * row) {

    _peerTreeViewRow = row;

    setPeerBitSwaprBEPSupportStatus(_peerPlugin->peerBitSwaprBEPSupportStatus());
}

void PeerPlugin::unsetPeerTreeViewRow() {
    _peerTreeViewRow = nullptr;
}

void PeerPlugin::setPeerBEP10SupportStatus(const extension::BEPSupportStatus &) {
    // not useful
}

void PeerPlugin::setPeerBitSwaprBEPSupportStatus(const extension::BEPSupportStatus & status) {

    if(_peerTreeViewRow != nullptr)
        _peerTreeViewRow->setBEPSupport(status);

}

}
}
}
