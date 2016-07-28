/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#include <core/PeerPlugin.hpp>

namespace joystream {
namespace core {

PeerPlugin::PeerPlugin(const extension::status::PeerPlugin & status)
    : _endPoint(status.endPoint)
    , _peerBEP10SupportStatus(status.peerBEP10SupportStatus)
    , _peerBitSwaprBEPSupportStatus(status.peerBitSwaprBEPSupportStatus) {
}

libtorrent::tcp::endpoint PeerPlugin::endPoint() const noexcept {
    return _endPoint;
}

extension::BEPSupportStatus PeerPlugin::peerBEP10SupportStatus() const noexcept {
    return _peerBEP10SupportStatus;
}

extension::BEPSupportStatus PeerPlugin::peerBitSwaprBEPSupportStatus() const noexcept {
    return _peerBitSwaprBEPSupportStatus;
}

void PeerPlugin::update(const extension::status::PeerPlugin & status) {

    if(_peerBEP10SupportStatus != status.peerBEP10SupportStatus) {
        _peerBEP10SupportStatus = status.peerBEP10SupportStatus;
        emit peerBEP10SupportStatusChanged(_peerBEP10SupportStatus);
    }

    if(_peerBitSwaprBEPSupportStatus != status.peerBitSwaprBEPSupportStatus) {
        _peerBitSwaprBEPSupportStatus = status.peerBitSwaprBEPSupportStatus;
        emit peerBitSwaprBEPSupportStatus(_peerBitSwaprBEPSupportStatus);
    }
}

}
}
