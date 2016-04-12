/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 21 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_DETAIL_INITIALIZESELLING_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_DETAIL_INITIALIZESELLING_HPP

#include <protocol_wire/SellerTerms.hpp>
#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {
namespace detail {

    class InitializeSelling : public sc::event<InitializeSelling> {

    public:

        InitializeSelling(const protocol_wire::SellerTerms &);

        // Getters and setters
        protocol_wire::SellerTerms terms() const;

    private:

        protocol_wire::SellerTerms _terms;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_DETAIL_INITIALIZESELLING_HPP
