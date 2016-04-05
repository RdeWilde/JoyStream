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
        : _modeAnnounced(ModeAnnounced::none)
        , _index(0) {
    }

    void PeerModeAnnounced::toObserve() {
        _modeAnnounced = ModeAnnounced::observe;
    }

    void PeerModeAnnounced::toBuy(const wire::BuyerTerms & t) {
        _modeAnnounced = ModeAnnounced::buy;
        _buyModeTerms = t;
    }

    void PeerModeAnnounced::toSell(const wire::SellerTerms & t, uint32_t i) {
        _modeAnnounced = ModeAnnounced::sell;
        _sellModeTerms = t;
        _index = i;
    }

    ModeAnnounced PeerModeAnnounced::modeAnnounced() const {
        return _modeAnnounced;
    }

    void PeerModeAnnounced::setModeAnnounced(const ModeAnnounced announced) {
        _modeAnnounced = announced;
    }

    wire::BuyerTerms PeerModeAnnounced::buyModeTerms() const {
        return _buyModeTerms;
    }

    void PeerModeAnnounced::setBuyModeTerms(const wire::BuyerTerms & t) {
        _buyModeTerms = t;
    }

    wire::SellerTerms PeerModeAnnounced::sellModeTerms() const {
        return _sellModeTerms;
    }

    void PeerModeAnnounced::setSellModeTerms(const wire::SellerTerms & t) {
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
