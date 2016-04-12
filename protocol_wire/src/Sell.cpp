/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/Sell.hpp>
#include <protocol_wire/MessageType.hpp>

#include <QDataStream>

namespace joystream {
namespace protocol_wire {

    Sell::Sell()
        : _index(0) {
    }

    Sell::Sell(const SellerTerms & terms, uint32_t index)
        : _terms(terms)
        , _index(index) {
    }

    Sell::Sell(QDataStream & stream) {

        quint64 price, lock, maxSellers, minContractFeePerKb, settlementFee;

        stream >> price >> lock >> maxSellers >> minContractFeePerKb >> settlementFee;

        _terms.setMinPrice(price);
        _terms.setMinLock(lock);
        _terms.setMaxSellers(maxSellers);
        _terms.setMinContractFeePerKb(minContractFeePerKb);
        _terms.setSettlementFee(settlementFee);
    }

    MessageType Sell::messageType() const {
        return MessageType::sell;
    }

    quint32 Sell::length() const {
        return sizeof(_terms.minPrice()) + sizeof(_terms.minLock()) + sizeof(_terms.maxSellers()) + sizeof(_terms.minContractFeePerKb()) + sizeof(_terms.settlementFee());
    }

    void Sell::write(QDataStream & stream) const {
        stream << _terms.minPrice() << _terms.minLock() << _terms.maxSellers() << _terms.minContractFeePerKb() << _terms.settlementFee();
    }

    SellerTerms Sell::terms() const  {
        return _terms;
    }

    void Sell::setTerms(const SellerTerms & terms) {
        _terms = terms;
    }

    uint32_t Sell::index() const {
        return _index;
    }

    void Sell::setIndex(uint32_t index) {
        _index = index;
    }
}
}
