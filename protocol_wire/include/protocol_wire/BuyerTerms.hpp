/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_WIREBUYERTERMS_HPP
#define JOYSTREAM_PROTOCOL_WIREBUYERTERMS_HPP

//#include <stdint.h>
#include <QtGlobal>
#include <common/RelativeLockTime.hpp>

namespace joystream {
namespace protocol_wire {

    class SellerTerms;

    // Terms in buy mode message
    class BuyerTerms {

    public:

        BuyerTerms();

        BuyerTerms(quint64 maxPrice, uint16_t maxLock, quint32 minNumberOfSellers, quint64 maxContractFeePerKb);

        bool operator==(const BuyerTerms & rhs) const;

        bool operator!=(const BuyerTerms & rhs) const;

        // Read from stream: is friend to write directly to private BuyerTerm members, simpler than using setters
        friend QDataStream & operator >>(QDataStream &, BuyerTerms &);

        bool satisfiedBy(const SellerTerms & terms) const;

        // Lenght of wire encoding
        static quint32 length();

        quint64 maxPrice() const;
        void setMaxPrice(quint64 maxPrice);

        uint16_t maxLock() const;
        void setMaxLock(const uint16_t &);

        quint32 minNumberOfSellers() const;
        void setMinNumberOfSellers(quint32 minNumberOfSellers);

        quint64 maxContractFeePerKb() const;
        void setMaxContractFeePerKb(quint64 maxContractFeePerKb);

    private:

        // Maximum price accepted (satoshies)
        //int64_t _maxPrice;
        quint64 _maxPrice;

        // Maximum Relative locktime (unit is defined at the protocol session layer)
        uint16_t _maxLock;

        // Minimum number of sellers
        //uint32_t _numberOfSellers;
        quint32 _minNumberOfSellers;

        // Maximum fee per byte in contract transaction (satoshies)
        //int64_t _maxFeePerKb;
        quint64 _maxContractFeePerKb;
    };

    // Write terms to stream
    QDataStream & operator <<(QDataStream &, const BuyerTerms &);

}
}

#endif // JOYSTREAM_PROTOCOL_WIREBUYERTERMS_HPP
