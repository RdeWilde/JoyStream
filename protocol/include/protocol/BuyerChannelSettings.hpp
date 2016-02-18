/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 18 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUYER_CHANNEL_SETTINGS_HPP
#define JOYSTREAM_PROTOCOL_BUYER_CHANNEL_SETTINGS_HPP

#include <common/KeyPair.hpp>

namespace joystream {

    namespace paymentchannel {
        class Channel;
    }

    namespace protocol {

        class BuyerChannelSettings {

        public:

            BuyerChannelSettings();

            joystream::paymentchannel::Channel toChannel() const;

        private:

            // Refund fee
            quint64 _refundFee;

            // Controls payour output of multisig
            Coin::KeyPair _payorContractKeyPair;

            // Controls final payment to payor
            Coin::KeyPair _payorFinalKeyPair;
        };
    }
}

#endif // JOYSTREAM_PROTOCOL_BUYER_CHANNEL_SETTINGS_HPP

