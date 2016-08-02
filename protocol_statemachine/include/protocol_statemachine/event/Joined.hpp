/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_JOINCONTRACT_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_JOINCONTRACT_HPP

#include <boost/statechart/event.hpp>
#include <common/KeyPair.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {
namespace event {

    class Joined : public sc::event<Joined> {

    public:

        Joined();
        Joined(const Coin::KeyPair &, const Coin::PubKeyHash &);

        // Getters
        Coin::KeyPair contractKeys() const;

        Coin::PubKeyHash finalPkHash() const;

    private:

        // Contract output payee keys
        Coin::KeyPair _contractKeys;

        // Payment payee output
        Coin::PubKeyHash _finalPkHash;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_JOINCONTRACT_HPP

