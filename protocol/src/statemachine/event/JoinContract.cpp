/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#include <protocol/statemachine/event/JoinContract.hpp>

using namespace joystream::protocol::statemachine::event;

JoinContract::JoinContract(const ContractRSVP & rsvp)
    : _rsvp(rsvp) {
}

joystream::protocol::ContractRSVP JoinContract::rsvp() const {
    return _rsvp;
}

void JoinContract::setRsvp(const ContractRSVP & rsvp){
    _rsvp = rsvp;
}
