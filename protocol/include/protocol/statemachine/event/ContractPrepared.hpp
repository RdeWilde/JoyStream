/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_CONTRACTPREPARED_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_CONTRACTPREPARED_HPP

#include <boost/statechart/event.hpp>
#include <common/typesafeOutPoint.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    class ContractPrepared : public sc::event<ContractPrepared> {

    public:

        ContractPrepared(const Coin::typesafeOutPoint &);

        Coin::typesafeOutPoint anchor() const;

    private:

        Coin::typesafeOutPoint _anchor;
    };

}
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_CONTRACTPREPARED_HPP

