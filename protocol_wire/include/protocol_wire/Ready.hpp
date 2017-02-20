/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_READY_HPP
#define JOYSTREAM_WIRE_READY_HPP

#include <protocol_wire/Message.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/PublicKey.hpp>

namespace joystream {
namespace protocol_wire {

    class Ready : public Message {

    public:

        Ready();
        Ready(uint64_t, const Coin::typesafeOutPoint &, const Coin::PublicKey &, const Coin::PubKeyHash &);

        virtual ~Ready() {}

        bool operator==(const Ready &) const;

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;

        // Getters
        uint64_t value() const;

        Coin::typesafeOutPoint anchor() const;

        Coin::PublicKey contractPk() const;

        Coin::PubKeyHash finalPkHash() const;

        // Getters
        void setValue(uint64_t);

        void setAnchor(const Coin::typesafeOutPoint&);

        void setContractPk(const Coin::PublicKey&);

        void setFinalPkHash(const Coin::PubKeyHash&);

    private:

        // Value locked up
        // NB: May be dropped later
        uint64_t _value;

        // Anchor for contract
        Coin::typesafeOutPoint _anchor;

        // Contract output buyer multisig key
        Coin::PublicKey _contractPk;

        // Payment/Refund buyer output
        Coin::PubKeyHash _finalPkHash;
    };

}
}

#endif // JOYSTREAM_WIRE_READY_HPP
