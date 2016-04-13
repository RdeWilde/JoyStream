/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_BUYMODESTARTED_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_BUYMODESTARTED_HPP

#include <boost/statechart/event.hpp>
#include <protocol_wire/protocol_wire.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {
namespace event {

    class BuyModeStarted : public sc::event<BuyModeStarted> {

    public:

        BuyModeStarted(const joystream::protocol_wire::BuyerTerms  & terms);

        // Getters and setters
        joystream::protocol_wire::BuyerTerms terms() const;
        void setTerms(const joystream::protocol_wire::BuyerTerms &terms);

    private:

        joystream::protocol_wire::BuyerTerms _terms;

    };

}
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_BUYMODESTARTED_HPP

