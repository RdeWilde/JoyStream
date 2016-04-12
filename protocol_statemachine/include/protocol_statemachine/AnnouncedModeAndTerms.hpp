/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_ANNOUNCEDMODEANDTERMS_HPP
#define JOYSTREAM_PROTOCOL_ANNOUNCEDMODEANDTERMS_HPP

#include <protocol_statemachine/ModeAnnounced.hpp>
#include <protocol_wire/BuyerTerms.hpp>
#include <protocol_wire/SellerTerms.hpp>

namespace joystream {
namespace protocol_statemachine {

    class AnnouncedModeAndTerms {

    public:

        AnnouncedModeAndTerms();

        // Mode change routines
        void toObserve();
        void toBuy(const protocol_wire::BuyerTerms &);
        void toSell(const protocol_wire::SellerTerms &, uint32_t );

        // Getters and setters
        ModeAnnounced modeAnnounced() const;
        void setModeAnnounced(const ModeAnnounced);

        protocol_wire::BuyerTerms buyModeTerms() const;
        void setBuyModeTerms(const protocol_wire::BuyerTerms &);

        protocol_wire::SellerTerms sellModeTerms() const;
        void setSellModeTerms(const protocol_wire::SellerTerms &);

        uint32_t index() const;
        void setIndex(uint32_t index);

    private:

        // Mode that has been announced
        ModeAnnounced _modeAnnounced;

        // Buy mode terms
        protocol_wire::BuyerTerms _buyModeTerms;

        // Sell mode terms
        protocol_wire::SellerTerms _sellModeTerms;

        // Sell mode index
        uint32_t _index;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_ANNOUNCEDMODEANDTERMS_HPP
