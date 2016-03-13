/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_PEER_MODE_ANNOUNCED_HPP
#define JOYSTREAM_PROTOCOL_PEER_MODE_ANNOUNCED_HPP

#include <protocol/BuyerTerms.hpp>
#include <protocol/SellerTerms.hpp>

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

        PeerModeAnnounced();

        PeerModeAnnounced(ModeAnnounced announced, const BuyerTerms & buyModeTerms, const SellerTerms & sellModeTerms);

        // Mode change routines
        void toObserve();
        void toBuyMode(const BuyerTerms &);
        void toSellMode(const SellerTerms &);

        // Getters and setters
        ModeAnnounced announced() const;
        void setAnnounced(const ModeAnnounced);

        BuyerTerms buyModeTerms() const;
        void setBuyModeTerms(const BuyerTerms &);

        SellerTerms sellModeTerms() const;
        void setSellModeTerms(const SellerTerms &);

    private:

        // Mode that has been announced
        ModeAnnounced _announced;

        // Buy mode terms
        BuyerTerms _buyModeTerms;

        // Sell mode terms
        SellerTerms _sellModeTerms;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_PEER_MODE_ANNOUNCED_HPP
