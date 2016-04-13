/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 24 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_INVITESELLER_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_INVITESELLER_HPP

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {
namespace event {

    class InviteSeller : public sc::event<InviteSeller> {

        InviteSeller();
    };

}
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_INVITESELLER_HPP
