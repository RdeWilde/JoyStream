/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 17 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELLER_HPP
#define JOYSTREAM_PROTOCOL_SELLER_HPP

#include <string>
#include <cstdlib>

namespace joystream {
namespace protocol {

    class Seller {

        enum class State {
            unassigned,
            invited,
            joined,
            signed_refund,
            removed
        };

    public:

        Seller();

        Seller(State state, const std::string & peerName, uint32_t channelIndex);

        // Getters and setters
        State state() const;
        void setState(State state);

        std::string peerName() const;
        void setPeerName(const std::string & peerName);

        uint32_t channelIndex() const;
        void setChannelIndex(uint32_t channelIndex);

    private:

        // State of this seller
        State _state;

        // Name of this seller (make template later)
        std::string _peerName;

        // Index of channel in payment channel
        uint32_t _channelIndex;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_SELLER_HPP
