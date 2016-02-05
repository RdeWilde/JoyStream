/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUY_PEER_STATE_HPP
#define JOYSTREAM_PROTOCOL_BUY_PEER_STATE_HPP

#include <protocol/wire/Sell.hpp>
#include <protocol/wire/JoiningContract.hpp>

namespace joystream {
namespace protocol {
namespace buy {

    // State of peer when client is in buy mode
    class PeerState {

    public:

        /**
        // Enumeration of possible states
        // a peer can have when facing buyer
        // and last message sent was
        // 1) welformed
        // 2) state compatible
        enum class LastValidAction {
            no_bitswapr_message_sent, // No message extended message beyond extended handshake has been sent
            mode_announced,
            joined_contract,
            signed_refund,
            sent_valid_piece
        };
        */

        // Bad states
        enum class FailureMode {
            not_failed,
            mode_message_time_out,
            join_contract_time_out,
            refund_signature_time_out,
            refund_incorrectly_signed,
            sent_invalid_piece, // wrong piece, or integrity not intact
        };

        // Default constructor
        PeerState();

        // Constructor from members
        PeerState(//LastValidAction lastAction,
                  FailureMode failureMode,
                  const joystream::protocol::wire::Sell & lastSellReceived,
                  const joystream::protocol::wire::JoiningContract & lastJoiningContractReceived);

        // Getters and setters

        //LastValidAction lastAction() const;
        //void setLastAction(LastValidAction lastAction);

        FailureMode failureMode() const;
        void setFailureMode(FailureMode failureMode);

        joystream::protocol::wire::Sell lastSellReceived() const;
        void setLastSellReceived(const joystream::protocol::wire::Sell & lastSellReceived);

        joystream::protocol::wire::JoiningContract lastJoiningContractReceived() const;
        void setLastJoiningContractReceived(const joystream::protocol::wire::JoiningContract & lastJoiningContractReceived);

    private:

        // Last valid action of peer
        //LastValidAction _lastAction;

        // How peer may have failed
        FailureMode _failureMode;

        // Last seller message
        joystream::protocol::wire::Sell _lastSellReceived;

        // Last joining contract message
        joystream::protocol::wire::JoiningContract _lastJoiningContractReceived;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_BUY_PEER_STATE_HPP

