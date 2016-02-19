/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUYER_TERMS_HPP
#define JOYSTREAM_PROTOCOL_BUYER_TERMS_HPP

//#include <stdint.h>
#include <QtGlobal>

namespace joystream {
namespace protocol {

    class SellerTerms;

    // Terms in buy mode message
    class BuyerTerms {

    public:

        BuyerTerms();

        BuyerTerms(quint64 maxPrice, quint32 maxLock, quint32 minNumberOfSellers, quint64 maxContractFeePerKb, quint64 refundFee);

        bool satisfiedBy(const SellerTerms & terms) const;

        quint64 maxPrice() const;
        void setMaxPrice(quint64 maxPrice);

        quint32 maxLock() const;
        void setMaxLock(quint32 maxLock);

        quint32 minNumberOfSellers() const;
        void setMinNumberOfSellers(quint32 minNumberOfSellers);

        quint64 maxContractFeePerKb() const;
        void setMaxContractFeePerKb(quint64 maxContractFeePerKb);

        quint64 refundFee() const;
        void setRefundFee(quint64 refundFee);

    private:

        // Maximum price accepted (satoshies)
        //int64_t _maxPrice;
        quint64 _maxPrice;

        // Maximum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
        //uint32_t _maxLock;
        quint32 _maxLock;

        // Minimum number of sellers
        //uint32_t _numberOfSellers;
        quint32 _minNumberOfSellers;

        // Maximum fee per byte in contract transaction (satoshies)
        //int64_t _maxFeePerKb;
        quint64 _maxContractFeePerKb;

        // Total fee (satoshies) for refund transaction
        quint64 _refundFee;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_BUYER_TERMS_HPP
