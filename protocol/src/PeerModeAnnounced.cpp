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
        : _announced(ModeAnnounced::none)
        , _index(0){
    }

    void PeerModeAnnounced::toObserve() {
        _announced = ModeAnnounced::observe;
    }

    void PeerModeAnnounced::toBuy(const BuyerTerms & t) {
        _announced = ModeAnnounced::buy;
        _buyModeTerms = t;
    }

    void PeerModeAnnounced::toSell(const SellerTerms & t, uint32_t i) {
        _announced = ModeAnnounced::sell;
        _sellModeTerms = t;
        _index = i;
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

    uint32_t PeerModeAnnounced::index() const {
        return _index;
    }

    void PeerModeAnnounced::setIndex(uint32_t index) {
        _index = index;
    }
}
}
