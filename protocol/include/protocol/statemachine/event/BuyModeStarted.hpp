/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_BUYMODESTARTED_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_BUYMODESTARTED_HPP

#include <boost/statechart/event.hpp>
#include <protocol/BuyerTerms.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    class BuyModeStarted : public sc::event<BuyModeStarted> {

    public:

        BuyModeStarted(const joystream::protocol::BuyerTerms  & terms);

        // Getters and setters
        joystream::protocol::BuyerTerms terms() const;
        void setTerms(const joystream::protocol::BuyerTerms &terms);

    private:

        joystream::protocol::BuyerTerms _terms;

    };

}
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_BUYMODESTARTED_HPP

