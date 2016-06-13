/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_CONTRACTPREPARED_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_CONTRACTPREPARED_HPP

#include <boost/statechart/event.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/KeyPair.hpp>
#include <common/RedeemScriptHash.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {
namespace event {

    class ContractPrepared : public sc::event<ContractPrepared> {

    public:

        ContractPrepared();
        ContractPrepared(const Coin::typesafeOutPoint &, const Coin::KeyPair &, const Coin::RedeemScriptHash &, quint64);

        // Getters
        Coin::typesafeOutPoint anchor() const;

        Coin::KeyPair contractKeyPair() const;

        Coin::RedeemScriptHash finalScriptHash() const;

        quint64 value() const;

    private:

        // Anchor for contract
        Coin::typesafeOutPoint _anchor;

        // Contract output buyer multisig key
        Coin::KeyPair _buyerContractKeyPair;

        // Payment/Refund buyer output
        Coin::RedeemScriptHash _finalScriptHash;

        // NB: may be temporary
        quint64 _value;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_EVENT_CONTRACTPREPARED_HPP
