/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 6 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SESSION_CONFIGURATION_HPP
#define JOYSTREAM_PROTOCOL_SESSION_CONFIGURATION_HPP

#include <protocol/Mode.hpp>
#include <protocol/sell/Terms.hpp>
#include <protocol/buy/Terms.hpp>


namespace joystream {
namespace protocol {

    //********
    // I dont htin this class is needed tbh.
    //*******
    class SessionConfiguration {

    public:

    private:

        // Mode
        Mode _mode;

        //////////////////
        /// BUYER MODE ///
        //////////////////

        buy::Terms _buyTerms;

        ///////////////////
        /// SELLER MODE ///
        ///////////////////

        sell::Terms _sellTerms;

        // Maximum time (s) for which seller is willing to seed without contract getting at least one confirmation
        //uint32_t _maxContractConfirmationDelay;

    };
}
}

#endif // JOYSTREAM_PROTOCOL_SESSION_CONFIGURATION_HPP

