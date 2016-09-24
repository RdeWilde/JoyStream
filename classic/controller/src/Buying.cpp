/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 24 2016
 */

#include <controller/Buying.hpp>
#include <controller/Seller.hpp>
#include <core/core.hpp>

namespace joystream {
namespace classic {
namespace controller {

Buying::Buying(core::Buying * buying)
    : _buying(buying) {
}

Buying::~Buying() {
}

void Buying::setFunding(const Coin::UnspentP2PKHOutput &) {

}

void Buying::setPolicy(const protocol_session::BuyingPolicy &) {

}

void Buying::setState(const protocol_session::BuyingState &) {

}

void Buying::setTerms(const protocol_wire::BuyerTerms &) {

}

void Buying::addSeller(core::Seller * seller) {

    if(_sellers.count(seller->connectionId()) > 0)
        throw std::runtime_error("Seller already added.");

    _sellers[seller->connectionId()] = std::unique_ptr<Seller>(new Seller(seller));
}

void Buying::removeSeller(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _sellers.find(endPoint);

    if(it == _sellers.cend())
        throw std::runtime_error("No such seller exists.");

    _sellers.erase(it);
}

void Buying::setContractTx(const Coin::Transaction &) {

}

}
}
}
