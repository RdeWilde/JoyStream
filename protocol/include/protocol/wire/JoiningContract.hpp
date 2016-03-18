/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_JOINING_CONTRACT_HPP
#define JOYSTREAM_PROTOCOL_WIRE_JOINING_CONTRACT_HPP

#include <protocol/wire/ExtendedMessagePayload.hpp>
#include <protocol/ContractRSVP.hpp>

namespace joystream {
namespace protocol {
namespace wire {

    class JoiningContract : public ExtendedMessagePayload {

    public:

        // Default constructor
        JoiningContract();

        // Construct from members
        JoiningContract(const ContractRSVP &);

        // Constructor based on raw payload
        // NB: Substitute with static factory in future, so that you cannot create stale
        // payload objects if there is an error in the reading from stream
        JoiningContract(QDataStream & stream);

        // Getter
        ContractRSVP rsvp() const;
        void setRsvp(const ContractRSVP &rsvp);

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual quint32 length() const;
        virtual void write(QDataStream & stream) const;

    private:

        // RSVP response to join contract
        ContractRSVP _rsvp;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_JOINING_CONTRACT_HPP
