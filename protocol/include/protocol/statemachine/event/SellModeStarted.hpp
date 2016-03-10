/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_SELLMODESTARTED_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_SELLMODESTARTED_HPP

#include <boost/statechart/event.hpp>
#include <protocol/SellerTerms.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    class SellModeStarted : public sc::event<SellModeStarted> {

    public:

        SellModeStarted(const joystream::protocol::SellerTerms & terms);

        // Getters and setters
        joystream::protocol::SellerTerms terms() const;
        void setTerms(const joystream::protocol::SellerTerms &terms);

    private:

        joystream::protocol::SellerTerms _terms;
    };

}
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_SELLMODESTARTED_HPP
