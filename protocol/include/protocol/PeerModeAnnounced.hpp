/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_PEER_MODE_ANNOUNCED_HPP
#define JOYSTREAM_PROTOCOL_PEER_MODE_ANNOUNCED_HPP

#include <protocol/ModeAnnounced.hpp>
#include <wire/BuyerTerms.hpp>
#include <wire/SellerTerms.hpp>

namespace joystream {
namespace protocol {

    class PeerModeAnnounced {

    public:

        PeerModeAnnounced();

        // Mode change routines
        void toObserve();
        void toBuy(const joystream::wire::BuyerTerms &);
        void toSell(const joystream::wire::SellerTerms &, uint32_t );

        // Getters and setters
        ModeAnnounced modeAnnounced() const;
        void setModeAnnounced(const ModeAnnounced);

        joystream::wire::BuyerTerms buyModeTerms() const;
        void setBuyModeTerms(const joystream::wire::BuyerTerms &);

        joystream::wire::SellerTerms sellModeTerms() const;
        void setSellModeTerms(const joystream::wire::SellerTerms &);

        uint32_t index() const;
        void setIndex(uint32_t index);

    private:

        // Mode that has been announced
        ModeAnnounced _modeAnnounced;

        // Buy mode terms
        joystream::wire::BuyerTerms _buyModeTerms;

        // Sell mode terms
        joystream::wire::SellerTerms _sellModeTerms;

        // Sell mode index
        uint32_t _index;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_PEER_MODE_ANNOUNCED_HPP
