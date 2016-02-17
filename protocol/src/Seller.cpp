/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 17 2016
 */

#include <protocol/Seller.hpp>

namespace joystream {
namespace protocol {

    Seller::Seller()
        : _state(State::unassigned)
        , _channelIndex(0) {
    }

    Seller::Seller(State state, const std::string & peerName, uint32_t channelIndex)
        : _state(state)
        , _peerName(peerName)
        , _channelIndex(channelIndex) {
    }

    Seller::State Seller::state() const {
        return _state;
    }

    void Seller::setState(State state) {
        _state = state;
    }

    std::string Seller::peerName() const {
        return _peerName;
    }

    void Seller::setPeerName(const std::string & peerName) {
        _peerName = peerName;
    }

    uint32_t Seller::channelIndex() const {
        return _channelIndex;
    }

    void Seller::setChannelIndex(uint32_t channelIndex) {
        _channelIndex = channelIndex;
    }
}
}
