/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 16 2016
 */
#include <protocol_wire/BuyerTerms.hpp>
#include <protocol_wire/SellerTerms.hpp>

#include <QDataStream>

namespace joystream {
namespace protocol_wire {

    BuyerTerms::BuyerTerms()
        : BuyerTerms(0,0,0,0,0) {
    }

    BuyerTerms::BuyerTerms(quint64 maxPrice, uint16_t maxLock, quint32 minNumberOfSellers, quint64 maxContractFeePerKb, quint64 refundFee)
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

    bool BuyerTerms::operator!=(const BuyerTerms & rhs) const {
        return !(*this == rhs);
    }

    QDataStream & operator >>(QDataStream & stream, BuyerTerms & rhs) {

        stream >> rhs._maxPrice >> rhs._maxLock >> rhs._minNumberOfSellers >> rhs._maxContractFeePerKb >> rhs._refundFee;

        return stream;
    }

    bool BuyerTerms::satisfiedBy(const SellerTerms & terms) const {
        return terms.satisfiedBy(*this);
    }

    quint32 BuyerTerms::length() {
        return sizeof(_maxPrice) + sizeof(_maxLock) + sizeof(_minNumberOfSellers) + sizeof(_maxContractFeePerKb) + sizeof(_refundFee);
    }

    quint64 BuyerTerms::maxPrice() const {
        return _maxPrice;
    }

    void BuyerTerms::setMaxPrice(quint64 maxPrice) {
        _maxPrice = maxPrice;
    }

    uint16_t BuyerTerms::maxLock() const {
        return _maxLock;
    }

    void BuyerTerms::setMaxLock(const uint16_t &maxLock) {
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

    QDataStream & operator <<(QDataStream & stream, const BuyerTerms & rhs) {

        stream << rhs.maxPrice() << rhs.maxLock() << rhs.minNumberOfSellers() << rhs.maxContractFeePerKb() << rhs.refundFee();
        return stream;
    }
}
}
