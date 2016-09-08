/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 17 2016
 */

#include <core/Selling.hpp>

Q_DECLARE_METATYPE(joystream::protocol_session::SellingPolicy)
Q_DECLARE_METATYPE(joystream::protocol_wire::SellerTerms)

namespace joystream {
namespace core {

void Selling::registerMetaTypes() {

    qRegisterMetaType<protocol_session::SellingPolicy>();
    qRegisterMetaType<protocol_wire::SellerTerms>();
}

Selling::Selling(const protocol_session::SellingPolicy & policy,
                 const protocol_wire::SellerTerms & terms)
    : _policy(policy)
    , _terms(terms) {
}

Selling * Selling::create(const protocol_session::status::Selling & status) {
    return new Selling(status.policy, status.terms);
}

protocol_session::SellingPolicy Selling::policy() const noexcept {
    return _policy;
}

protocol_wire::SellerTerms Selling::terms() const noexcept {
    return _terms;
}

void Selling::update(const protocol_session::status::Selling & status) {

    if(_policy != status.policy) {

        _policy = status.policy;
        emit policyChanged(_policy);
    }

    if(_terms != status.terms) {

        _terms = status.terms;
        emit termsChanged(_terms);
    }

}

}
}
