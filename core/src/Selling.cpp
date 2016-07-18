/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 17 2016
 */

#include <core/Selling.hpp>

namespace joystream {
namespace core {

Selling::Selling(const protocol_session::status::Selling & status)
    : _policy(status.policy)
    , _terms(status.terms) {
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
