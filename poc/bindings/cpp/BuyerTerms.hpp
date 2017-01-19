/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 9 2017
 */

#ifndef JOYSTREAM_NODE_BUYERTERMS_HPP
#define JOYSTREAM_NODE_BUYERTERMS_HPP

#include <nan.h>

namespace joystream {
namespace protocol_wire {
  class BuyerTerms;
}
namespace node {

  const static char * MAX_PRICE_KEY = "maxPrice";
  const static char * MAX_LOCK_KEY = "maxLock";
  const static char * MIN_NUMBER_OF_SELLERS_KEY = "minNumberOfSellers";
  const static char * MAX_CONTRACT_FEE_PER_KB_KEY = "maxContractFeePerKb";

  /**
   * Encoding o
   * {Number} o.maxPric - maximum unit price in sats.
   * {Number} o.maxLock - maximum lock time in seconds.
   * {Number} o.minNumberOfSellers - min. number of sellers.
   * {Number} o.maxContractFeePerKb - max. pr. Kb contract tx fee in stats.
   */

  v8::Local<v8::Object> createObject(const protocol_wire::BuyerTerms & terms);
  protocol_wire::BuyerTerms fromObject(const v8::Local<v8::Object> & o);

}
}

#endif // JOYSTREAM_NODE_BUYERTERMS_HPP
