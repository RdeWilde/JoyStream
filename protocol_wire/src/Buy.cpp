/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/Buy.hpp>
#include <protocol_wire/MessageType.hpp>

#include <QDataStream>

namespace joystream {
namespace protocol_wire {

    Buy::Buy() {
    }

    Buy::Buy(const BuyerTerms & terms)
        : _terms(terms) {
    }

    Buy::Buy(QDataStream & stream) {

        quint64 maxPrice, maxLock, minNumberOfSellers, maxContractFeePerKb, refundFee;

        stream >> maxPrice >> maxLock >> minNumberOfSellers >> maxContractFeePerKb >> refundFee;

        _terms.setMaxPrice(maxPrice);
        _terms.setMaxLock(maxLock);
        _terms.setMinNumberOfSellers(minNumberOfSellers);
        _terms.setMaxContractFeePerKb(maxContractFeePerKb);
        _terms.setRefundFee(refundFee);
    }

    MessageType Buy::messageType() const {
        return MessageType::buy;
    }

    quint32 Buy::length() const {
        return sizeof(_terms.maxPrice()) + sizeof(_terms.maxLock()) + sizeof(_terms.minNumberOfSellers()) + sizeof(_terms.minNumberOfSellers()) + sizeof(_terms.maxContractFeePerKb()) + sizeof(_terms.refundFee());
    }

    void Buy::write(QDataStream & stream) const {
        stream << _terms.maxPrice() << _terms.maxLock() << _terms.minNumberOfSellers() << _terms.maxContractFeePerKb() << _terms.refundFee();
    }
    
    BuyerTerms Buy::terms() const {
        return _terms;
    }
    
    void Buy::setTerms(const BuyerTerms & terms) {
        _terms = terms;
    }

}
}
