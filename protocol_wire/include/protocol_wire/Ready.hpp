/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_READY_HPP
#define JOYSTREAM_WIRE_READY_HPP

#include <protocol_wire/ExtendedMessagePayload.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/PublicKey.hpp>
#include <common/RedeemScriptHash.hpp>

namespace joystream {
namespace protocol_wire {

    class Ready : public ExtendedMessagePayload {

    public:

        Ready();
        Ready(quint64, const Coin::typesafeOutPoint &, const Coin::PublicKey &, const Coin::RedeemScriptHash &);
        Ready(QDataStream &);

        virtual ~Ready() {}

        bool operator==(const Ready &) const;

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual quint32 length() const;
        virtual void write(QDataStream &) const;

        // Getters
        quint64 value() const;

        Coin::typesafeOutPoint anchor() const;

        Coin::PublicKey contractPk() const;

        Coin::RedeemScriptHash finalScriptHash() const;

    private:

        // Value locked up
        // NB: May be dropped later
        quint64 _value;

        // Anchor for contract
        Coin::typesafeOutPoint _anchor;

        // Contract output buyer multisig key
        Coin::PublicKey _contractPk;

        // Payment/Refund buyer output
        Coin::RedeemScriptHash _finalScriptHash;
    };

}
}

#endif // JOYSTREAM_WIRE_READY_HPP
