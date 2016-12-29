/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#include <core/PeerPlugin.hpp>

Q_DECLARE_METATYPE(joystream::extension::BEPSupportStatus)

namespace joystream {
namespace core {

void PeerPlugin::registerMetaTypes() {

    qRegisterMetaType<extension::BEPSupportStatus>();
}

PeerPlugin::PeerPlugin(const libtorrent::tcp::endpoint & endPoint,
                       const extension::BEPSupportStatus & peerBEP10SupportStatus,
                       const extension::BEPSupportStatus & peerBitSwaprBEPSupportStatus)
    : _endPoint(endPoint)
    , _peerBEP10SupportStatus(peerBEP10SupportStatus)
    , _peerBitSwaprBEPSupportStatus(peerBitSwaprBEPSupportStatus) {
}

PeerPlugin * PeerPlugin::create(const extension::status::PeerPlugin & status) {
    return new PeerPlugin(status.endPoint,
                          status.peerBEP10SupportStatus,
                          status.peerBitSwaprBEPSupportStatus);
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
        emit peerBitSwaprBEPSupportStatusChanged(_peerBitSwaprBEPSupportStatus);
    }
}

}
}
