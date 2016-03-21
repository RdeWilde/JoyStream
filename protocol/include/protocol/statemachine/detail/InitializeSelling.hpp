/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 21 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_DETAIL_INITIALIZESELLING_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_DETAIL_INITIALIZESELLING_HPP

#include <protocol/SellerTerms.hpp>
#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {
namespace detail {

    class InitializeSelling : public sc::event<InitializeSelling> {

    public:

        InitializeSelling(const SellerTerms &);

        // Getters and setters
        SellerTerms terms() const;

    private:

        SellerTerms _terms;
    };

}
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_DETAIL_INITIALIZESELLING_HPP
