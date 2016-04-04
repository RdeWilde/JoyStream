/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_JOIN_CONTRACT_HPP
#define JOYSTREAM_WIRE_JOIN_CONTRACT_HPP

#include <wire/ExtendedMessagePayload.hpp>
#include <wire/ContractInvitation.hpp>

namespace joystream {
namespace wire {

    class JoinContract : public ExtendedMessagePayload {

    public:

        JoinContract();

        JoinContract(const ContractInvitation & invitation, uint32_t index);

        // Constructor based on raw payload
        JoinContract(QDataStream & stream);

        // Virtual methods that subclassing messages have to implement
        MessageType messageType() const;
        quint32 length() const;
        void write(QDataStream & stream) const;

        // Getters and setters
        ContractInvitation invitation() const;
        void setInvitation(const ContractInvitation & invitation);

        uint32_t index() const;
        void setIndex(uint32_t index);

    private:

        // Invitation to contract
        ContractInvitation _invitation;

        // Seller terms index
        uint32_t _index;
    };

}
}

#endif // JOYSTREAM_WIRE_JOIN_CONTRACT_HPP
