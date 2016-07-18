/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#include <protocol_statemachine/AnnouncedModeAndTerms.hpp>

namespace joystream {
namespace protocol_statemachine {

    AnnouncedModeAndTerms::AnnouncedModeAndTerms()
        : _modeAnnounced(ModeAnnounced::none)
        , _index(0) {
    }

    bool AnnouncedModeAndTerms::operator==(const AnnouncedModeAndTerms & rhs) const {

        // Make sure mode is the same
        if(_modeAnnounced != rhs.modeAnnounced())
            return false;

        if(_modeAnnounced == ModeAnnounced::buy)
            return _buyModeTerms == rhs.buyModeTerms();
        else if(_modeAnnounced == ModeAnnounced::sell)
            return _sellModeTerms == rhs.sellModeTerms() && _index == rhs.index();
        else
            return true; // if mode is not set, or is obseve, then we are done
    }

    void AnnouncedModeAndTerms::toObserve() {
        _modeAnnounced = ModeAnnounced::observe;
    }

    void AnnouncedModeAndTerms::toBuy(const protocol_wire::BuyerTerms & t) {
        _modeAnnounced = ModeAnnounced::buy;
        _buyModeTerms = t;
    }

    void AnnouncedModeAndTerms::toSell(const protocol_wire::SellerTerms & t, uint32_t i) {
        _modeAnnounced = ModeAnnounced::sell;
        _sellModeTerms = t;
        _index = i;
    }

    ModeAnnounced AnnouncedModeAndTerms::modeAnnounced() const {
        return _modeAnnounced;
    }

    void AnnouncedModeAndTerms::setModeAnnounced(const ModeAnnounced announced) {
        _modeAnnounced = announced;
    }

    protocol_wire::BuyerTerms AnnouncedModeAndTerms::buyModeTerms() const {
        return _buyModeTerms;
    }

    void AnnouncedModeAndTerms::setBuyModeTerms(const protocol_wire::BuyerTerms & t) {
        _buyModeTerms = t;
    }

    protocol_wire::SellerTerms AnnouncedModeAndTerms::sellModeTerms() const {
        return _sellModeTerms;
    }

    void AnnouncedModeAndTerms::setSellModeTerms(const protocol_wire::SellerTerms & t) {
        _sellModeTerms = t;
    }

    uint32_t AnnouncedModeAndTerms::index() const {
        return _index;
    }

    void AnnouncedModeAndTerms::setIndex(uint32_t index) {
        _index = index;
    }
}
}
