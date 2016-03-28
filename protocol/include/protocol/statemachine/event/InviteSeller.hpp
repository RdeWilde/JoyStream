/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 24 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_INVITESELLER_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_INVITESELLER_HPP

#include <common/KeyPair.hpp>
#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    class InviteSeller : public sc::event<InviteSeller> {

    public:

        InviteSeller(quint64, const Coin::KeyPair &, const Coin::PubKeyHash &);

        // Getters
        quint64 value() const;

        Coin::KeyPair buyerContractKeyPair() const;

        Coin::PubKeyHash finalPkHash() const;

    private:

        // Here we could also do ContractInvitation + private key for contract

        // Contract output value
        // cant be int64_t, due to compatibility wiht QDataStream
        quint64 _value;

        // Controls buyer portiono of contract output
        Coin::KeyPair _buyerContractKeyPair;

        // Payment/Refund buyer output
        Coin::PubKeyHash _finalPkHash;
    };

}
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_INVITESELLER_HPP
