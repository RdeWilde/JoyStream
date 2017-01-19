/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 11 2017
 */

#include "SellerTerms.hpp"
#include "utils.hpp"

#include <protocol_wire/protocol_wire.hpp>

namespace joystream {
namespace node {
namespace SellerTerms {

v8::Local<v8::Object> createObject(const protocol_wire::SellerTerms & terms) {

  v8::Local<v8::Object> o = Nan::New<v8::Object>();

  SET_NUMBER(o, MIN_PRICE_KEY, terms.minPrice());
  SET_UINT32(o, MIN_LOCK_KEY, terms.minLock());
  SET_UINT32(o, MAX_NUMBER_OF_SELLERS_KEY, terms.maxSellers());
  SET_NUMBER(o, MIN_CONTRACT_FEE_PER_KB_KEY, terms.minContractFeePerKb());
  SET_NUMBER(o, SETTLEMENT_FEE_KEY, terms.settlementFee());
}

protocol_wire::SellerTerms fromObject(const v8::Local<v8::Object> & o) {

  protocol_wire::SellerTerms terms;

  try {

    // Desired type: quint64
    terms.setMinPrice(GET_INT64(o, MIN_PRICE_KEY));

    // Desired type: uint16_t
    terms.setMinLock(GET_UINT32(o, MIN_LOCK_KEY));

    // Desired type: quint32
    terms.setMaxSellers(GET_UINT32(o, MAX_NUMBER_OF_SELLERS_KEY));

    // Desired type: quint64
    terms.setMinContractFeePerKb(GET_INT64(o, MIN_CONTRACT_FEE_PER_KB_KEY));

    // Desired type: quint64
    terms.setSettlementFee(GET_INT64(o, SETTLEMENT_FEE_KEY));

  } catch (const std::runtime_error & e) {

      std::string errorString = std::string("Could not construct seller terms from given arguments: ") + e.what();

      Nan::ThrowError(Nan::New(errorString).ToLocalChecked());
  }

  return terms;
}

}
}
}
