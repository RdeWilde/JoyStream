/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#include <protocol/statemachine/event/PieceLoaded.hpp>

using namespace joystream::protocol::statemachine::event;

PieceLoaded::PieceLoaded(const boost::shared_array<char> & data, int length)
    : _data(data)
    , _length(length) {
}

boost::shared_array<char> PieceLoaded::data() const {
    return _data;
}

int PieceLoaded::length() const {
    return _length;
}
