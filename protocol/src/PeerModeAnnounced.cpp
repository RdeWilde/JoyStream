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

    void PeerModeAnnounced::toObserve() {
        _announced = ModeAnnounced::observe;
    }

    void PeerModeAnnounced::toBuyMode(const BuyerTerms & t) {
        _announced = ModeAnnounced::buy;
        _buyModeTerms = t;
    }

    void PeerModeAnnounced::toSellMode(const SellerTerms & t) {
        _announced = ModeAnnounced::sell;
        _sellModeTerms = t;
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

    void PeerModeAnnounced::setBuyModeTerms(const BuyerTerms & t) {
        _buyModeTerms = t;
    }

    SellerTerms PeerModeAnnounced::sellModeTerms() const {
        return _sellModeTerms;
    }

    void PeerModeAnnounced::setSellModeTerms(const SellerTerms & t) {
        _sellModeTerms = t;
    }    
}
}
