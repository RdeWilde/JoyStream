/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_SELL_HPP
#define JOYSTREAM_PROTOCOL_WIRE_SELL_HPP

#include <protocol/wire/ExtendedMessagePayload.hpp>
#include <protocol/SellerTerms.hpp>

namespace joystream {
namespace protocol {
namespace wire {

    class Sell : public ExtendedMessagePayload {

    public:

        // Default constructor
        Sell();

        // Constructor based on members
        Sell(const joystream::protocol::SellerTerms & terms, uint32_t index);

        // Constructor based on raw data
        Sell(QDataStream & stream);

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual quint32 length() const;
        virtual void write(QDataStream & stream) const;

        // Getters and setters
        joystream::protocol::SellerTerms terms() const;
        void setTerms(const joystream::protocol::SellerTerms & terms);

        uint32_t index() const;
        void setIndex(uint32_t index);

    private:

        // Sales terms
        joystream::protocol::SellerTerms _terms;

        // Terms index
        uint32_t _index;
    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_SELL_HPP
