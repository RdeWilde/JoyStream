/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 21 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_DETAIL_INITIALIZEBUYING_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_DETAIL_INITIALIZEBUYING_HPP

#include <protocol_wire/protocol_wire.hpp>
#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {
namespace detail {

    class InitializeBuying : public sc::event<InitializeBuying> {

    public:

        InitializeBuying(const joystream::protocol_wire::BuyerTerms &);

        // Getters and setters
        joystream::protocol_wire::BuyerTerms terms() const;

    private:

        joystream::protocol_wire::BuyerTerms _terms;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_DETAIL_INITIALIZEBUYING_HPP
