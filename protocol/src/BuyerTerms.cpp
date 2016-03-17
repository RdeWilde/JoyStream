/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 16 2016
 */
#include <protocol/BuyerTerms.hpp>
#include <protocol/SellerTerms.hpp>

namespace joystream {
namespace protocol {

    BuyerTerms::BuyerTerms() {
    }

    BuyerTerms::BuyerTerms(quint64 maxPrice, quint32 maxLock, quint32 minNumberOfSellers, quint64 maxContractFeePerKb, quint64 refundFee)
        : _maxPrice(maxPrice)
        , _maxLock(maxLock)
        , _minNumberOfSellers(minNumberOfSellers)
        , _maxContractFeePerKb(maxContractFeePerKb)
        , _refundFee(refundFee) {
    }

    bool BuyerTerms::operator==(const BuyerTerms & rhs) const {
        return _maxPrice == rhs.maxPrice() &&
               _maxLock == rhs.maxLock() &&
               _minNumberOfSellers == rhs.minNumberOfSellers() &&
               _maxContractFeePerKb == rhs.maxContractFeePerKb() &&
               _refundFee == rhs.refundFee();
    }

    bool BuyerTerms::satisfiedBy(const SellerTerms & terms) const {
        return terms.satisfiedBy(*this);
    }

    quint64 BuyerTerms::maxPrice() const {
        return _maxPrice;
    }

    void BuyerTerms::setMaxPrice(quint64 maxPrice) {
        _maxPrice = maxPrice;
    }

    quint32 BuyerTerms::maxLock() const {
        return _maxLock;
    }

    void BuyerTerms::setMaxLock(quint32 maxLock) {
        _maxLock = maxLock;
    }

    quint32 BuyerTerms::minNumberOfSellers() const {
        return _minNumberOfSellers;
    }

    void BuyerTerms::setMinNumberOfSellers(quint32 minNumberOfSellers) {
        _minNumberOfSellers = minNumberOfSellers;
    }

    quint64 BuyerTerms::maxContractFeePerKb() const {
        return _maxContractFeePerKb;
    }

    void BuyerTerms::setMaxContractFeePerKb(quint64 maxContractFeePerKb) {
        _maxContractFeePerKb = maxContractFeePerKb;
    }

    quint64 BuyerTerms::refundFee() const {
        return _refundFee;
    }

    void BuyerTerms::setRefundFee(quint64 refundFee) {
        _refundFee = refundFee;
    }
}
}
