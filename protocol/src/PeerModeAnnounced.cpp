/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#include <protocol/PeerModeAnnounced.hpp>

namespace joystream {
namespace protocol {

    PeerModeAnnounced::PeerModeAnnounced()
        : _announced(ModeAnnounced::none) {
    }

    PeerModeAnnounced::PeerModeAnnounced(ModeAnnounced announced, const BuyerTerms & buyModeTerms, const SellerTerms & sellModeTerms)
        : _announced(announced)
        , _buyModeTerms(buyModeTerms)
        , _sellModeTerms(sellModeTerms) {
    }

    PeerModeAnnounced::ModeAnnounced PeerModeAnnounced::announced() const {
        return _announced;
    }

    void PeerModeAnnounced::setAnnounced(const ModeAnnounced announced) {
        _announced = announced;
    }

    BuyerTerms PeerModeAnnounced::buyModeTerms() const {
        return _buyModeTerms;
    }

    void PeerModeAnnounced::setBuyModeTerms(const BuyerTerms & buyModeTerms) {
        _buyModeTerms = buyModeTerms;
    }

    SellerTerms PeerModeAnnounced::sellModeTerms() const {
        return _sellModeTerms;
    }

    void PeerModeAnnounced::setSellModeTerms(const SellerTerms & sellModeTerms) {
        _sellModeTerms = sellModeTerms;
    }    
}
}
