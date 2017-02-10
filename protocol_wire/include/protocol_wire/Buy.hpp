/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_BUY_HPP
#define JOYSTREAM_PROTOCOL_WIRE_BUY_HPP

#include <protocol_wire/ExtendedMessagePayload.hpp>
#include <protocol_wire/BuyerTerms.hpp>

namespace joystream {
namespace protocol_wire {

    class Buy : public ExtendedMessagePayload {

    public:

        // Default constructor
        Buy();

        // Constructor based on members
        Buy(const BuyerTerms & terms);

        virtual ~Buy() {}

        bool operator==(const Buy &) const;

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;

        // Getters and setters
        BuyerTerms terms() const;
        void setTerms(const BuyerTerms &);

        // Overload the << operator for easiness in debugging with GTest
        friend std::ostream& operator<<(std::ostream &os, const Buy &obj);

    private:

        // Buyer terms
        BuyerTerms _terms;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_BUY_HPP
