/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_SELL_HPP
#define JOYSTREAM_WIRE_SELL_HPP

#include <protocol_wire/ExtendedMessagePayload.hpp>
#include <protocol_wire/SellerTerms.hpp>

namespace joystream {
namespace protocol_wire {

    class Sell : public ExtendedMessagePayload {

    public:

        // Default constructor
        Sell();

        // Constructor based on members
        Sell(const SellerTerms & terms, uint32_t index);

        // Constructor based on raw data
        Sell(std::istream & stream);

        virtual ~Sell() {}

        bool operator==(const Sell &) const;

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual quint32 length() const;
        virtual void write(std::ostream & stream) const;

        // Getters and setters
        SellerTerms terms() const;
        void setTerms(const SellerTerms & terms);

        uint32_t index() const;
        void setIndex(uint32_t index);

    private:

        // Sales terms
        SellerTerms _terms;

        // Terms index
        uint32_t _index;
    };
}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_SELL_HPP
