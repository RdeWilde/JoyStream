/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_SELL_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_SELL_HPP

#include <protocol/statemachine/Active.hpp>
#include <protocol/statemachine/detail/InitializeSelling.hpp>
#include <protocol/statemachine/event/ObserveModeStarted.hpp>
#include <protocol/statemachine/event/BuyModeStarted.hpp>
#include <protocol/statemachine/event/UpdateTerms.hpp>
#include <paymentchannel/Payee.hpp>

//#include <protocol/ContractInvitation.hpp>

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
                                sc::custom_reaction<detail::InitializeSelling>,
                                sc::custom_reaction<event::ObserveModeStarted>,
                                sc::custom_reaction<event::BuyModeStarted>,
                                sc::custom_reaction<event::UpdateTerms<joystream::wire::SellerTerms>>
                                > reactions;

        Selling();

        // Event handlers
        sc::result react(const detail::InitializeSelling &);
        sc::result react(const event::ObserveModeStarted &);
        sc::result react(const event::BuyModeStarted &);
        sc::result react(const event::UpdateTerms<joystream::wire::SellerTerms> &);

        // Getters
        uint32_t index() const;

    private:

        // Sub states which require access to private variables
        friend class ReadyForInvitation;
        friend class Invited;
        friend class ReadyForPieceRequest;
        friend class ServicingPieceRequest;
        friend class WaitingToStart;
        friend class WaitingForPayment;

        // Updates _payee based on terms
        void updatePayeeTerms(const joystream::wire::SellerTerms &);

        // Whether state has been initialized with detail::InitializeSelling
        bool _initialized;

        //// Client state

        // Most recent terms broadcastded
        //SellerTerms _terms;

        // Index for most recent terms broadcasted
        uint32_t _index;

        // Contract invitation received
        //ContractInvitation _invitation;

        // Payee side of payment channel interaction
        paymentchannel::Payee _payee;
    };
}
}
}

// Required to make Selling complete when included throught his header file,
// as ReadyForInvitation is initial state and thus part of parent state_machine definition
#include <protocol/statemachine/ReadyForInvitation.hpp>

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_SELL_HPP
