/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_JOINCONTRACT_HPP
#define JOYSTREAM_PROTOCOL_WIRE_JOINCONTRACT_HPP

#include <protocol_wire/ExtendedMessagePayload.hpp>

namespace joystream {
namespace protocol_wire {

    class JoinContract : public ExtendedMessagePayload {

    public:

        JoinContract();
        JoinContract(uint32_t index);
        JoinContract(std::istream & stream);

        virtual ~JoinContract() {}

        bool operator==(const JoinContract &) const;

        // Virtual methods that subclassing messages have to implement
        MessageType messageType() const;
        uint32_t length() const;
        void write(std::ostream & stream) const;

        // Getters
        uint32_t index() const;

    private:

        // Seller terms index
        // cant be uint32_t, due to qdatastream not supporting it
        uint32_t _index;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_JOINCONTRACT_HPP
