/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_SELL_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_SELL_HPP

#include <protocol_statemachine/Active.hpp>
#include <protocol_statemachine/event/ObserveModeStarted.hpp>
#include <protocol_statemachine/event/BuyModeStarted.hpp>
#include <protocol_statemachine/event/UpdateTerms.hpp>

namespace Coin {
    class typesafeOutPoint;
    class PublicKey;
}

namespace joystream {
namespace protocol_statemachine {

    // Initial state for Selling
    class ReadyForInvitation;

    class Selling : public sc::simple_state<Selling, Active, ReadyForInvitation> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::ObserveModeStarted>,
                                sc::custom_reaction<event::BuyModeStarted>,
                                sc::custom_reaction<event::UpdateTerms<protocol_wire::SellerTerms>>
                                > reactions;

        Selling();

        // Event handlers
        sc::result react(const event::ObserveModeStarted &);
        sc::result react(const event::BuyModeStarted &);
        sc::result react(const event::UpdateTerms<protocol_wire::SellerTerms> &);

    };
}
}

// Required to make Selling complete when included throught his header file,
// as ReadyForInvitation is initial state and thus part of parent state_machine definition
#include <protocol_statemachine/ReadyForInvitation.hpp>

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_SELL_HPP
