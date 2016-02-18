/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 18 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUYER_PAYOR_SETTINGS_HPP
#define JOYSTREAM_PROTOCOL_BUYER_PAYOR_SETTINGS_HPP

#include <protocol/BuyerChannelSettings.hpp>


namespace joystream {
namespace protocol {

    //
    class BuyerPayorSettings {

    public:

    private:

        std::vector<BuyerChannelSettings> _channelSettings;

        Coin::UnspentP2PKHOutput & utxo;

        Coin::KeyPair changeOutputKeyPair;

        quint64 changeValue;
    };
}
}

#endif // JOYSTREAM_PROTOCOL_BUYER_PAYOR_SETTINGS_HPP

