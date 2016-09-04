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

CBStateMachine::CBStateMachine(const std::type_index & innerStateTypeIndex,
                               const protocol_statemachine::AnnouncedModeAndTerms & announcedModeAndTermsFromPeer,
                               Payor * payor,
                               Payee * payee)
    : _innerStateTypeIndex(innerStateTypeIndex)
    , _announcedModeAndTermsFromPeer(announcedModeAndTermsFromPeer)
    , _payor(payor)
    , _payee(payee) {
}

CBStateMachine * create(const protocol_session::status::CBStateMachine & status) {
    return new CBStateMachine(status.innerStateTypeIndex,
                              status.announcedModeAndTermsFromPeer,
                              Payor::create(status.payor),
                              Payee::create(status.payee));
}

CBStateMachine::~CBStateMachine() {
}

std::type_index CBStateMachine::innerStateTypeIndex() const noexcept {
    return _innerStateTypeIndex;
}

protocol_statemachine::AnnouncedModeAndTerms CBStateMachine::announcedModeAndTermsFromPeer() const noexcept {
    return _announcedModeAndTermsFromPeer;
}

Payor * CBStateMachine::payor() const noexcept {
    return _payor.get();
}

Payee * CBStateMachine::payee() const noexcept {
    return _payee.get();
}

void CBStateMachine::update(const protocol_session::status::CBStateMachine & status) {

    if(_innerStateTypeIndex != status.innerStateTypeIndex) {
        _innerStateTypeIndex = status.innerStateTypeIndex;
        emit innerStateTypeIndexChanged(_innerStateTypeIndex);
    }

    if(_announcedModeAndTermsFromPeer != status.announcedModeAndTermsFromPeer) {
        _announcedModeAndTermsFromPeer = status.announcedModeAndTermsFromPeer;
        emit announcedModeAndTermsFromPeerChanged(_announcedModeAndTermsFromPeer);
    }

    _payor->update(status.payor);
    _payee->update(status.payee);
}

}
}
