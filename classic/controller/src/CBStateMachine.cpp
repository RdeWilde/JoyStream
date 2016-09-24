/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 16 2016
 */

#include <controller/CBStateMachine.hpp>
#include <core/core.hpp>

namespace joystream {
namespace classic {
namespace controller {

CBStateMachine::CBStateMachine(core::CBStateMachine * CBStateMachine)
    : _CBStateMachine(CBStateMachine)
    , _payor(_CBStateMachine->payor())
    , _payee(_CBStateMachine->payee()) {
}

Payor & CBStateMachine::payor() noexcept {
    return _payor;
}

Payee & CBStateMachine::payee() noexcept {
    return _payee;
}

void CBStateMachine::setAnnouncedModeAndTermsFromPeer(const protocol_statemachine::AnnouncedModeAndTerms & announcedModeAndTerms) {
    // do something with gui objects
}

}
}
}
