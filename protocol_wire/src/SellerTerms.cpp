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

namespace joystream {
namespace protocol_wire {

    SellerTerms::SellerTerms()
        : SellerTerms(0,0,0,0,0) {
    }

    SellerTerms::SellerTerms(uint64_t price, uint16_t minLock, uint32_t maxSellers, uint64_t minContractFeePerKb, uint64_t settlementFee)
        : _minPrice(price)
        , _minLock(minLock)
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

    std::istream & operator >>(std::istream & stream, SellerTerms & rhs) {

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
    uint32_t SellerTerms::length() {
        return sizeof(_minPrice) + sizeof(_minLock)+ sizeof(_maxSellers) + sizeof(_minContractFeePerKb) + sizeof(_settlementFee);
    }

    uint64_t SellerTerms::minPrice() const {
        return _minPrice;
    }

    void SellerTerms::setMinPrice(uint64_t price) {
        _minPrice = price;
    }

    uint16_t SellerTerms::minLock() const {
        return _minLock;
    }

    void SellerTerms::setMinLock(const uint16_t &minLock) {
        _minLock = minLock;
    }

    uint32_t SellerTerms::maxSellers() const {
        return _maxSellers;
    }

    void SellerTerms::setMaxSellers(uint32_t maxSellers) {
        _maxSellers = maxSellers;
    }

    uint64_t SellerTerms::minContractFeePerKb() const {
        return _minContractFeePerKb;
    }

    void SellerTerms::setMinContractFeePerKb(uint64_t minContractFeePerKb) {
        _minContractFeePerKb = minContractFeePerKb;
    }

    uint64_t SellerTerms::settlementFee() const {
        return _settlementFee;
    }

    void SellerTerms::setSettlementFee(uint64_t settlementFee) {
        _settlementFee = settlementFee;
    }

    std::ostream & operator <<(std::ostream & stream, const SellerTerms & rhs) {

        stream << rhs.minPrice() << rhs.minLock() << rhs.maxSellers() << rhs.minContractFeePerKb() << rhs.settlementFee();
        return stream;
    }

}
}
