/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELLER_TERMS_HPP
#define JOYSTREAM_PROTOCOL_SELLER_TERMS_HPP

//#include <stdint.h>

#include <functional>
#include <QtGlobal>

namespace joystream {
namespace protocol {

    class BuyerTerms;

    // Terms in sell mode message
    class SellerTerms {

    public:

        enum class OrderingPolicy {
            random,
            min_price
            // do something more complex in the future for more complex tradeoffs
        };

        SellerTerms();

        SellerTerms(quint64 minPrice, quint32 lock, quint32 maxSellers, quint64 minContractFeePerKb, quint64 settlementFee);

        // Policy contingent comparison of terms
        static bool compare(OrderingPolicy policy, const SellerTerms & lhs, const SellerTerms & rhs);

        // Comparator bound to given policy, is useful for STL based sorting
        std::function<bool (const SellerTerms & lhs, const SellerTerms & rhs)> comparator(OrderingPolicy policy);

        bool satisfiedBy(const BuyerTerms & terms) const;

        quint64 minPrice() const;
        void setMinPrice(quint64 minPrice);

        quint32 lock() const;
        void setLock(quint32 lock);

        quint32 maxSellers() const;
        void setMaxSellers(quint32 maxSellers);

        quint64 minContractFeePerKb() const;
        void setMinContractFeePerKb(quint64 minContractFeePerKb);

        quint64 settlementFee() const;
        void setSettlementFee(quint64 settlementFee);

    private:

        // Piece price (satoshies)
        //int64_t _price;
        quint64 _minPrice;

        // When refund is spendable at the earliest (***UNITS?***)
        //uint32_t _lock;
        quint32 _minLock;

        // Maximum number of sellers accepted in contract
        //uint32_t _maxSellers;
        quint32 _maxSellers;

        // Maximum fee (satoshies) per byte in contract transaction
        //int64_t _maxFeePerKb;
        quint64 _minContractFeePerKb;

        // Total fee (satoshies) for settlement of contract
        quint64 _settlementFee;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_SELLER_TERMS_HPP
