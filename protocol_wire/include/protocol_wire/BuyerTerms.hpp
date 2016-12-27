/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_WIREBUYERTERMS_HPP
#define JOYSTREAM_PROTOCOL_WIREBUYERTERMS_HPP

#include <cstdint>
#include <iostream>

#include <common/RelativeLockTime.hpp>

namespace joystream {
namespace protocol_wire {

    class SellerTerms;

    // Terms in buy mode message
    class BuyerTerms {

    public:

        BuyerTerms();

        BuyerTerms(uint64_t maxPrice, uint16_t maxLock, uint32_t minNumberOfSellers, uint64_t maxContractFeePerKb);

        bool operator==(const BuyerTerms & rhs) const;

        bool operator!=(const BuyerTerms & rhs) const;

        // Read from stream: is friend to write directly to private BuyerTerm members, simpler than using setters
        friend std::istream & operator >>(std::istream &, BuyerTerms &);

        bool satisfiedBy(const SellerTerms & terms) const;

        // Lenght of wire encoding
        static uint32_t length();

        uint64_t maxPrice() const;
        void setMaxPrice(uint64_t maxPrice);

        uint16_t maxLock() const;
        void setMaxLock(const uint16_t &);

        uint32_t minNumberOfSellers() const;
        void setMinNumberOfSellers(uint32_t minNumberOfSellers);

        uint64_t maxContractFeePerKb() const;
        void setMaxContractFeePerKb(uint64_t maxContractFeePerKb);

    private:

        // Maximum price accepted (satoshies)
        //int64_t _maxPrice;
        uint64_t _maxPrice;

        // Maximum Relative locktime (unit is defined at the protocol session layer)
        uint16_t _maxLock;

        // Minimum number of sellers
        //uint32_t _numberOfSellers;
        uint32_t _minNumberOfSellers;

        // Maximum fee per byte in contract transaction (satoshies)
        //int64_t _maxFeePerKb;
        uint64_t _maxContractFeePerKb;
    };

    // Write terms to stream
    std::ostream & operator <<(std::ostream &, const BuyerTerms &);

}
}

#endif // JOYSTREAM_PROTOCOL_WIREBUYERTERMS_HPP
