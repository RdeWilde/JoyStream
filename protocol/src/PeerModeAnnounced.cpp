/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#include <protocol/PeerModeAnnounced.hpp>

namespace joystream {
namespace protocol {

    PeerModeAnnounced::PeerModeAnnounced(ModeAnnounced announced, const buy::Terms & buyModeTerms, const sell::Terms & sellModeTerms)
        : _announced(announced)
        , _buyModeTerms(buyModeTerms)
        , _sellModeTerms(sellModeTerms) {
    }

    buy::Terms PeerModeAnnounced::buyModeTerms() const {
        return _buyModeTerms;
    }

    void PeerModeAnnounced::setBuyModeTerms(const buy::Terms & buyModeTerms) {
        _buyModeTerms = buyModeTerms;
    }

    sell::Terms PeerModeAnnounced::sellModeTerms() const {
        return _sellModeTerms;
    }

    void PeerModeAnnounced::setSellModeTerms(const sell::Terms & sellModeTerms) {
        _sellModeTerms = sellModeTerms;
    }

}
}
