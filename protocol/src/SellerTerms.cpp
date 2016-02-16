/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 16 2016
 */

#include <protocol/SellerTerms.hpp>
#include <cassert>

namespace joystream {
namespace protocol {

    SellerTerms::SellerTerms()
        : SellerTerms(0, 0, 0, 0, 0) {
    }

    SellerTerms::SellerTerms(quint64 price, quint32 lock, quint32 maxSellers, quint64 minContractFeePerKb, quint64 settlementFee)
        : _price(price)
        , _lock(lock)
        , _maxSellers(maxSellers)
        , _minContractFeePerKb(minContractFeePerKb)
        , _settlementFee(settlementFee) {
    }

    bool SellerTerms::compare(OrderingPolicy policy, const SellerTerms & lhs, const SellerTerms & rhs) {

        switch(policy) {
            case OrderingPolicy::random: throw std::runtime_error("not yet implemented");
            case OrderingPolicy::min_price: return lhs.price() < rhs.price();
            default:
                assert(false);
        }
    }

    std::function<bool (const SellerTerms & lhs, const SellerTerms & rhs)> SellerTerms::comparator(OrderingPolicy policy) {

        //std::bind(SellerTerms::compare, )
        //SellerTerms::comparator()
    }

    quint64 SellerTerms::price() const {
        return _price;
    }

    void SellerTerms::setPrice(quint64 price) {
        _price = price;
    }

    quint32 SellerTerms::lock() const {
        return _lock;
    }

    void SellerTerms::setLock(quint32 lock) {
        _lock = lock;
    }

    quint32 SellerTerms::maxSellers() const {
        return _maxSellers;
    }

    void SellerTerms::setMaxSellers(quint32 maxSellers) {
        _maxSellers = maxSellers;
    }

    quint64 SellerTerms::minContractFeePerKb() const {
        return _minContractFeePerKb;
    }

    void SellerTerms::setMinContractFeePerKb(quint64 minContractFeePerKb) {
        _minContractFeePerKb = minContractFeePerKb;
    }

    quint64 SellerTerms::settlementFee() const {
        return _settlementFee;
    }

    void SellerTerms::setSettlementFee(quint64 settlementFee) {
        _settlementFee = settlementFee;
    }

}
}
