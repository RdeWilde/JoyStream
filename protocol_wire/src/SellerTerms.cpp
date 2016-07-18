/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 16 2016
 */

#include <protocol_wire/SellerTerms.hpp>
#include <protocol_wire/BuyerTerms.hpp>
#include <cassert>
#include <stdexcept>

#include <QDataStream>

namespace joystream {
namespace protocol_wire {

    SellerTerms::SellerTerms()
        : SellerTerms(0,0,0,0,0) {
    }

    SellerTerms::SellerTerms(quint64 price, quint32 lock, quint32 maxSellers, quint64 minContractFeePerKb, quint64 settlementFee)
        : _minPrice(price)
        , _minLock(lock)
        , _maxSellers(maxSellers)
        , _minContractFeePerKb(minContractFeePerKb)
        , _settlementFee(settlementFee) {
    }

    bool SellerTerms::operator==(const SellerTerms & rhs) const {
        return _minPrice == rhs.minPrice() &&
               _minLock == rhs.minLock() &&
               _maxSellers == rhs.maxSellers() &&
               _minContractFeePerKb == rhs.minContractFeePerKb() &&
               _settlementFee == rhs.settlementFee();
    }

    bool SellerTerms::operator!=(const SellerTerms & rhs) const {
        return !(*this == rhs);
    }

    QDataStream & operator >>(QDataStream & stream, SellerTerms & rhs) {

        stream >> rhs._minPrice >> rhs._minLock >> rhs._maxSellers >> rhs._minContractFeePerKb >> rhs._settlementFee;

        return stream;
    }

    bool SellerTerms::compare(OrderingPolicy policy, const SellerTerms & lhs, const SellerTerms & rhs) {

        switch(policy) {
            case OrderingPolicy::random: throw std::runtime_error("not yet implemented");
            case OrderingPolicy::min_price: return lhs.minPrice() < rhs.minPrice();
            default:
                assert(false);
        }
    }

    /**
    std::function<bool (const SellerTerms & lhs, const SellerTerms & rhs)> SellerTerms::comparator(OrderingPolicy policy) {

        //std::bind(SellerTerms::compare, )
        //SellerTerms::comparator()
    }
    */

    bool SellerTerms::satisfiedBy(const BuyerTerms & terms) const {
        return _minPrice <= terms.maxPrice() &&
               _minLock <= terms.maxLock() &&
               _maxSellers >= terms.minNumberOfSellers() &&
               _minContractFeePerKb <= terms.maxContractFeePerKb();
    }

    // Lenght of wire encoding
    quint32 SellerTerms::length() {
        return sizeof(_minPrice) + sizeof(_minLock) + sizeof(_maxSellers) + sizeof(_minContractFeePerKb) + sizeof(_settlementFee);
    }

    quint64 SellerTerms::minPrice() const {
        return _minPrice;
    }

    void SellerTerms::setMinPrice(quint64 price) {
        _minPrice = price;
    }

    quint32 SellerTerms::minLock() const {
        return _minLock;
    }

    void SellerTerms::setMinLock(quint32 minLock) {
        _minLock = minLock;
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

    QDataStream & operator <<(QDataStream & stream, const SellerTerms & rhs) {

        stream << rhs.minPrice() << rhs.minLock() << rhs.maxSellers() << rhs.minContractFeePerKb() << rhs.settlementFee();
        return stream;
    }

}
}
