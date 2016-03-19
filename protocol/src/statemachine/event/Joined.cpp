/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#include <protocol/statemachine/event/Joined.hpp>

using namespace joystream::protocol::statemachine::event;

Joined::Joined(const ContractRSVP & rsvp)
    : _rsvp(rsvp) {
}

joystream::protocol::ContractRSVP Joined::rsvp() const {
    return _rsvp;
}

void Joined::setRsvp(const ContractRSVP & rsvp){
    _rsvp = rsvp;
}
