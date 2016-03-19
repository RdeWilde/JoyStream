/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_SELL_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_SELL_HPP

#include <protocol/statemachine/Active.hpp>
#include <protocol/statemachine/event/UpdateTerms.hpp>
#include <protocol/ContractInvitation.hpp>

namespace Coin {
    class typesafeOutPoint;
    class PublicKey;
}

namespace joystream {
namespace protocol {
namespace statemachine {

    // Initial state for Selling
    class ReadyForInvitation;

    class Selling : public sc::simple_state<Selling, Active, ReadyForInvitation> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::ObserveModeStarted>,
                                sc::custom_reaction<event::BuyModeStarted>,
                                sc::custom_reaction<event::UpdateTerms<SellerTerms>>
                                > reactions;

        Selling();

        // Event handlers
        sc::result react(const event::ObserveModeStarted &);
        sc::result react(const event::BuyModeStarted &);
        sc::result react(const event::UpdateTerms<SellerTerms> &);

        // Getters
        uint32_t index() const;

    private:

        //// Client state

        // Terms
        uint32_t _index;

        // Contract invitation received
        ContractInvitation _invitation;
    };
}
}
}

// Required to make Selling complete when included throught his header file,
// as ReadyForInvitation is initial state and thus part of parent state_machine definition
#include <protocol/statemachine/ReadyForInvitation.hpp>

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_SELL_HPP
