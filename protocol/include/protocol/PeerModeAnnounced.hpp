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

        // Mode change routines
        void toObserve();
        void toBuy(const BuyerTerms &);
        void toSell(const SellerTerms &, uint32_t );

        // Getters and setters
        ModeAnnounced modeAnnounced() const;
        void setModeAnnounced(const ModeAnnounced);

        BuyerTerms buyModeTerms() const;
        void setBuyModeTerms(const BuyerTerms &);

        SellerTerms sellModeTerms() const;
        void setSellModeTerms(const SellerTerms &);

        uint32_t index() const;
        void setIndex(uint32_t index);

    private:

        // Mode that has been announced
        ModeAnnounced _modeAnnounced;

        // Buy mode terms
        BuyerTerms _buyModeTerms;

        // Sell mode terms
        SellerTerms _sellModeTerms;

        // Sell mode index
        uint32_t _index;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_PEER_MODE_ANNOUNCED_HPP
