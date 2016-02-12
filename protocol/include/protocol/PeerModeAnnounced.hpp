/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_PEER_MODE_ANNOUNCED_HPP
#define JOYSTREAM_PROTOCOL_PEER_MODE_ANNOUNCED_HPP

#include <protocol/buy/Terms.hpp>
#include <protocol/sell/Terms.hpp>

namespace joystream {
namespace protocol {

    class PeerModeAnnounced {

    public:

        enum class ModeAnnounced {
            none,
            observe,
            sell,
            buy
        };

        PeerModeAnnounced(ModeAnnounced announced, const buy::Terms & buyModeTerms, const sell::Terms & sellModeTerms);

        ModeAnnounced announced() const;
        void setAnnounced(const ModeAnnounced announced);

        buy::Terms buyModeTerms() const;
        void setBuyModeTerms(const buy::Terms & buyModeTerms);

        sell::Terms sellModeTerms() const;
        void setSellModeTerms(const sell::Terms & sellModeTerms);

    private:

        // Mode that has been announced
        ModeAnnounced _announced;

        // Buy mode terms
        buy::Terms _buyModeTerms;

        // Sell mode terms
        sell::Terms _sellModeTerms;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_PEER_MODE_ANNOUNCED_HPP
