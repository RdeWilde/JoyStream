/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 1 2016
 */

#include <protocol/Sell.hpp>
#include <protocol/JoiningContract.hpp>

#ifndef JOYSTREAM_EXTENSION_BUYER_PEER_PLUGIN_PEER_STATE_HPP
#define JOYSTREAM_EXTENSION_BUYER_PEER_PLUGIN_PEER_STATE_HPP

namespace joystream {
namespace extension {

    class BuyerPeerPluginPeerState {

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
        BuyerPeerPluginPeerState();

        // Constructor from members
        BuyerPeerPluginPeerState(//LastValidAction lastAction,
                  FailureMode failureMode,
                  const joystream::protocol::Sell & lastSellReceived,
                  const joystream::protocol::JoiningContract & lastJoiningContractReceived);

        // Getters and setters

        //LastValidAction lastAction() const;
        //void setLastAction(LastValidAction lastAction);

        FailureMode failureMode() const;
        void setFailureMode(FailureMode failureMode);

        joystream::protocol::Sell lastSellReceived() const;
        void setLastSellReceived(const joystream::protocol::Sell & lastSellReceived);

        joystream::protocol::JoiningContract lastJoiningContractReceived() const;
        void setLastJoiningContractReceived(const joystream::protocol::JoiningContract & lastJoiningContractReceived);

    private:

        // Last valid action of peer
        //LastValidAction _lastAction;

        // How peer may have failed
        FailureMode _failureMode;

        // Last seller message
        joystream::protocol::Sell _lastSellReceived;

        // Last joining contract message
        joystream::protocol::JoiningContract _lastJoiningContractReceived;
    };

}
}

#endif // JOYSTREAM_EXTENSION_BUYER_PEER_PLUGIN_PEER_STATE_HPP
