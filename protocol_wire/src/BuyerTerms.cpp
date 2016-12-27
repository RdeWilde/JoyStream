/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 16 2016
 */
#include <protocol_wire/BuyerTerms.hpp>
#include <protocol_wire/SellerTerms.hpp>

namespace joystream {
namespace protocol_wire {

    BuyerTerms::BuyerTerms()
        : BuyerTerms(0,0,0,0) {
    }

    BuyerTerms::BuyerTerms(uint64_t maxPrice, uint16_t maxLock, uint32_t minNumberOfSellers, uint64_t maxContractFeePerKb)
        : _maxPrice(maxPrice)
        , _maxLock(maxLock)
        , _minNumberOfSellers(minNumberOfSellers)
        , _maxContractFeePerKb(maxContractFeePerKb) {
    }

    bool BuyerTerms::operator==(const BuyerTerms & rhs) const {
        return _maxPrice == rhs.maxPrice() &&
               _maxLock == rhs.maxLock() &&
               _minNumberOfSellers == rhs.minNumberOfSellers() &&
               _maxContractFeePerKb == rhs.maxContractFeePerKb();
    }

    bool BuyerTerms::operator!=(const BuyerTerms & rhs) const {
        return !(*this == rhs);
    }

    std::istream & operator >>(std::istream & stream, BuyerTerms & rhs) {

        stream >> rhs._maxPrice >> rhs._maxLock >> rhs._minNumberOfSellers >> rhs._maxContractFeePerKb;

        return stream;
    }

    bool BuyerTerms::satisfiedBy(const SellerTerms & terms) const {
        return terms.satisfiedBy(*this);
    }

    uint32_t BuyerTerms::length() {
        return sizeof(_maxPrice) + sizeof(_maxLock) + sizeof(_minNumberOfSellers) + sizeof(_maxContractFeePerKb);
    }

    uint64_t BuyerTerms::maxPrice() const {
        return _maxPrice;
    }

    void BuyerTerms::setMaxPrice(uint64_t maxPrice) {
        _maxPrice = maxPrice;
    }

    uint16_t BuyerTerms::maxLock() const {
        return _maxLock;
    }

    void BuyerTerms::setMaxLock(const uint16_t &maxLock) {
        _maxLock = maxLock;
    }

    uint32_t BuyerTerms::minNumberOfSellers() const {
        return _minNumberOfSellers;
    }

    void BuyerTerms::setMinNumberOfSellers(uint32_t minNumberOfSellers) {
        _minNumberOfSellers = minNumberOfSellers;
    }

    uint64_t BuyerTerms::maxContractFeePerKb() const {
        return _maxContractFeePerKb;
    }

    void BuyerTerms::setMaxContractFeePerKb(uint64_t maxContractFeePerKb) {
        _maxContractFeePerKb = maxContractFeePerKb;
    }

    std::ostream & operator <<(std::ostream & stream, const BuyerTerms & rhs) {

        stream << rhs.maxPrice() << rhs.maxLock() << rhs.minNumberOfSellers() << rhs.maxContractFeePerKb();
        return stream;
    }
}
}
