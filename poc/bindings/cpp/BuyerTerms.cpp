/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 9 2017
 */

#include "BuyerTerms.hpp"
#include "utils.hpp"

#include <protocol_wire/protocol_wire.hpp>

namespace joystream {
namespace node {
namespace BuyerTerms {

v8::Local<v8::Object> createObject(const protocol_wire::BuyerTerms & terms) {

  v8::Local<v8::Object> o = Nan::New<v8::Object>();

  SET_NUMBER(o, MAX_PRICE_KEY, terms.maxPrice());
  SET_UINT32(o, MAX_LOCK_KEY, terms.maxLock());
  SET_UINT32(o, MIN_NUMBER_OF_SELLERS_KEY, terms.minNumberOfSellers());
  SET_NUMBER(o, MAX_CONTRACT_FEE_PER_KB_KEY, terms.maxContractFeePerKb());

}

protocol_wire::BuyerTerms fromObject(const v8::Local<v8::Object> & o) {

  protocol_wire::BuyerTerms terms;

  try {

    // Desired type: quint64
    terms.setMaxPrice(GET_INT64(o, MAX_PRICE_KEY));

    // Desired type: uint16_t
    terms.setMaxLock(GET_UINT32(o, MAX_LOCK_KEY));

    // Desired type: quint32
    terms.setMinNumberOfSellers(GET_UINT32(o, MIN_NUMBER_OF_SELLERS_KEY));

    // Desired type: quint64
    terms.setMaxContractFeePerKb(GET_INT64(o, MAX_CONTRACT_FEE_PER_KB_KEY));

  } catch (const std::runtime_error & e) {

    std::string errorString = std::string("Could not construct buyer terms from given arguments: ") + e.what();

    Nan::ThrowError(Nan::New(errorString).ToLocalChecked());
  }

  return terms;
}

}
}
}
