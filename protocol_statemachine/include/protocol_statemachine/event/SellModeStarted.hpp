/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_SELLMODESTARTED_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_SELLMODESTARTED_HPP

#include <boost/statechart/event.hpp>
#include <protocol_wire/protocol_wire.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {
namespace event {

    class SellModeStarted : public sc::event<SellModeStarted> {

    public:

        SellModeStarted(const joystream::protocol_wire::SellerTerms & terms);

        // Getters and setters
        joystream::protocol_wire::SellerTerms terms() const;
        void setTerms(const joystream::protocol_wire::SellerTerms &terms);

    private:

        joystream::protocol_wire::SellerTerms _terms;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_SELLMODESTARTED_HPP
