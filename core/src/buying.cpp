/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 18 2016
 */

#include <core/buying.hpp>

namespace joystream {
namespace core {

Buying::Buying(const protocol_session::status::Buying<libtorrent::tcp::endpoint> & status)
    : _funding(status.funding)
    , _policy(status.policy)
    , _state(status.state)
    , _terms(status.terms)
    , _contractTx(status.contractTx) {

    // Create sellers
    for(auto m : status.sellers)
        addSeller(m.second);
}


Coin::UnspentP2PKHOutput Buying::funding() const noexcept {
    return _funding;
}

protocol_session::BuyingPolicy Buying::policy() const noexcept {
    return _policy;
}

protocol_session::BuyingState Buying::state() const noexcept {
    return _state;
}

protocol_wire::BuyerTerms Buying::terms() const noexcept {
    return _terms;
}

std::map<libtorrent::tcp::endpoint, std::shared_ptr<Seller>> Buying::sellers() const noexcept {
    return _sellers;
}

Coin::Transaction Buying::contractTx() const noexcept {
    return _contractTx;
}

void Buying::addSeller(const protocol_session::status::Seller<libtorrent::tcp::endpoint> & s) {

    // if the seller alreadye exists, then ignore
    if(_sellers.count(s.connection) > 0)
        return;

    // Create seller
    std::shared_ptr<Seller> plugin(new Seller(s));

    // Add to map
    _sellers.insert(std::make_pair(s.connection, plugin));

    // announce
    emit sellerAdded(plugin);
}

void Buying::removeSeller(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _sellers.find(endPoint);

    // Ignore if it is already gone
    if(it != _sellers.cend())
        return;

    // Remove from map
    _sellers.erase(it);

    // announce
    emit sellerRemoved(endPoint);
}

void Buying::update(const protocol_session::status::Buying<libtorrent::tcp::endpoint> & status) {

    if(_funding != status.funding) {
        _funding = status.funding;
        emit fundingChanged(_funding);
    }

    if(_policy != status.policy) {
        _policy = status.policy;
        emit policyChanged(_policy);
    }

    if(_state != status.state) {
        _state = status.state;
        emit stateChanged(_state);
    }

    if(_terms != status.terms) {
        _terms = status.terms;
        emit termsChanged(_terms);
    }

    /**
    if(_contractTx != status.contractTx) {
        _contractTx = status.contractTx;
        emit contractTxChanged(_contractTx);
    }
    */

    /// Update sellers

    // for each seller with a status
    for(auto p: status.sellers) {

        auto it = _sellers.find(p.first);

        // if seller is present, then update
        if(it != _sellers.cend())
            it->second->update(p.second);
        else // otherwise add
            addSeller(p.second);
    }

    // for each existing seller
    for(auto p : _sellers) {

        // if there is no status for it, then remove
        if(status.sellers.count(p.first) == 0)
            removeSeller(p.first);
    }

}

}
}
