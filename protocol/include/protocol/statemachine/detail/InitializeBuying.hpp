/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 21 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_DETAIL_INITIALIZEBUYING_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_DETAIL_INITIALIZEBUYING_HPP

#include <protocol/BuyerTerms.hpp>
#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {
namespace detail {

    class InitializeBuying : public sc::event<InitializeBuying> {

    public:

        InitializeBuying(const BuyerTerms &);

        // Getters and setters
        BuyerTerms terms() const;

    private:

        BuyerTerms _terms;
    };

}
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_DETAIL_INITIALIZEBUYING_HPP
