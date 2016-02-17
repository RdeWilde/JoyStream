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

            // No seller yet assigned
            unassigned,

            // We have sent sign_refund message
            waiting_for_refund_signature,

            // Postponed sendining ready message since not all signatures were ready
            waiting_for_full_set_after_receiving_a_valid_refund_signature,

            // Sent ready message
            // NOT REALLY NEEDED, WE NEVER WAIT FOR ANYTHIN AFTER THIS
            //announced_ready,

            // At least one request message has been sent, for which no piece message
            // has yet been returned
            //waiting_for_requests_to_be_serviced,

            // Not been assigned piece
            waiting_to_be_assigned_piece,

            // We have requested a full piece, and we are waiting for it to arrive
            waiting_for_full_piece,

            // We are waiting for libtorrent to fire on_piece_pass() or on_piece_failed()
            // on a full piece which was recently received
            waiting_for_to_validate_and_store_piece
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
        // or make it a BuyerConnection *.
        std::string _peerName;

        // Index of channel in payment channel
        uint32_t _channelIndex;

        // Point in time when we last asked for a refund to be signed
        time_t _whenLastAskedForRefundSignature;

        //
        uint32_t _indexOfAssignedPiece;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_SELLER_HPP
