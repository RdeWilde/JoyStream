/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 18 2016
 */

#include <core/Buying.hpp>
#include <core/Seller.hpp>
#include <core/detail/detail.hpp>

Q_DECLARE_METATYPE(Coin::UnspentOutputSet)
Q_DECLARE_METATYPE(joystream::protocol_session::BuyingPolicy)
Q_DECLARE_METATYPE(joystream::protocol_session::BuyingState)
Q_DECLARE_METATYPE(joystream::protocol_wire::BuyerTerms)
Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)
Q_DECLARE_METATYPE(Coin::Transaction)

namespace joystream {
namespace core {

void Buying::registerMetaTypes() {

    qRegisterMetaType<Coin::UnspentOutputSet>();
    qRegisterMetaType<protocol_session::BuyingPolicy>();
    qRegisterMetaType<protocol_session::BuyingState>();
    qRegisterMetaType<protocol_wire::BuyerTerms>();
    qRegisterMetaType<libtorrent::tcp::endpoint>();
    qRegisterMetaType<Coin::Transaction>();
    Seller::registerMetaTypes();
}

Buying::Buying(const Coin::UnspentOutputSet & funding,
               const protocol_session::BuyingPolicy & policy,
               const protocol_session::BuyingState & state,
               const protocol_wire::BuyerTerms & terms,
               const Coin::Transaction & contractTx)
    : _funding(funding)
    , _policy(policy)
    , _state(state)
    , _terms(terms)
    , _contractTx(contractTx) {

}

Buying * Buying::create(const protocol_session::status::Buying<libtorrent::tcp::endpoint> & status) {

    Buying * buying = new Buying(status.funding,
                                 status.policy,
                                 status.state,
                                 status.terms,
                                 status.contractTx);

    for(auto m : status.sellers)
        buying->addSeller(m.second);

    return buying;
}

Buying::~Buying() {

    for(auto it = _sellers.cbegin();it != _sellers.cend();)
        removeSeller(it++);
}


Coin::UnspentOutputSet Buying::funding() const noexcept {
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

std::map<libtorrent::tcp::endpoint, Seller *> Buying::sellers() const noexcept {
    return detail::getRawMap<libtorrent::tcp::endpoint, Seller>(_sellers);
}

Coin::Transaction Buying::contractTx() const noexcept {
    return _contractTx;
}

void Buying::addSeller(const protocol_session::status::Seller<libtorrent::tcp::endpoint> & status) {

    assert(_sellers.count(status.connection) > 0);

    // Create seller
    Seller * s = Seller::create(status);

    // Add to map
    _sellers.insert(std::make_pair(status.connection, std::unique_ptr<Seller>(s)));

    // announce
    emit sellerAdded(s);
}

void Buying::removeSeller(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _sellers.find(endPoint);
    assert(it != _sellers.cend());

    removeSeller(it);
}

void Buying::removeSeller(std::map<libtorrent::tcp::endpoint, std::unique_ptr<Seller>>::const_iterator it) {

    libtorrent::tcp::endpoint endPoint = it->first;

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
    for(auto it = _sellers.cbegin(); it != _sellers.cend(); ) {

        // if there is no status for it, then remove
        if(status.sellers.count(it->first) == 0){
            removeSeller(it++);
	}else{
	    it++;
	}
    }

}

}
}
