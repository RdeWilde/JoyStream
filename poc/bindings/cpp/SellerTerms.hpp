/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 11 2017
 */

#ifndef JOYSTREAM_NODE_ADDON_SELLERTERMS_HPP
#define JOYSTREAM_NODE_ADDON_SELLERTERMS_HPP

#include <nan.h>
#include <protocol_wire/protocol_wire.hpp>

namespace joystream {
namespace node_addon {

class SellerTerms: public Nan::ObjectWrap {

    public:

      static NAN_MODULE_INIT(Init);
      static v8::Local<v8::Object> NewInstance(const protocol_wire::SellerTerms & terms);

    private:

      SellerTerms(const protocol_wire::SellerTerms & terms);

      protocol_wire::SellerTerms _terms;

      static Nan::Persistent<v8::Function> constructor;
      static NAN_METHOD(New);

      static NAN_GETTER(minPrice);
      static NAN_GETTER(minLock);
      static NAN_GETTER(maxSellers);
      static NAN_GETTER(minContractFeePerKb);
      static NAN_GETTER(settlementFee);

};

}
}

#endif
