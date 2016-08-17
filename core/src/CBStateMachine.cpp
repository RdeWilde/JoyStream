/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 17 2016
 */

#include <core/CBStateMachine.hpp>
#include <core/Payor.hpp>
#include <core/Payee.hpp>
#include <protocol_session/protocol_session.hpp>

namespace joystream {
namespace core {

CBStateMachine::CBStateMachine(const protocol_session::status::CBStateMachine & status)
    : _announcedModeAndTermsFromPeer(status.announcedModeAndTermsFromPeer)
    , _payor(new Payor(status.payor))
    , _payee(new Payee(status.payee)) {

}

protocol_statemachine::AnnouncedModeAndTerms CBStateMachine::announcedModeAndTermsFromPeer() const noexcept {
    return _announcedModeAndTermsFromPeer;
}

std::shared_ptr<Payor> CBStateMachine::payor() const noexcept {
    return _payor;
}

std::shared_ptr<Payee> CBStateMachine::payee() const noexcept {
    return _payee;
}

void CBStateMachine::update(const protocol_session::status::CBStateMachine & status) {

    if(_announcedModeAndTermsFromPeer != status.announcedModeAndTermsFromPeer) {
        _announcedModeAndTermsFromPeer = status.announcedModeAndTermsFromPeer;
        emit announcedModeAndTermsFromPeerChanged(_announcedModeAndTermsFromPeer);
    }

    _payor->update(status.payor);
    _payee->update(status.payee);
}

}
}
