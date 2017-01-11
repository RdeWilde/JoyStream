/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 9 2017
 */

#ifndef JOYSTREAM_NODE_ADDON_BUYERTERMS_HPP
#define JOYSTREAM_NODE_ADDON_BUYERTERMS_HPP

#include <nan.h>
#include <protocol_wire/protocol_wire.hpp>

namespace joystream {
namespace node_addon {

class BuyerTerms: public Nan::ObjectWrap {

    public:

      static NAN_MODULE_INIT(Init);
      static v8::Local<v8::Object> NewInstance(const protocol_wire::BuyerTerms & terms);

    private:

      BuyerTerms(const protocol_wire::BuyerTerms & terms);

      protocol_wire::BuyerTerms _terms;

      static Nan::Persistent<v8::Function> constructor;
      static NAN_METHOD(New);

      static NAN_GETTER(maxPrice);
      static NAN_GETTER(maxLock);
      static NAN_GETTER(minNumberOfSellers);
      static NAN_GETTER(maxContractFeePerKb);

};

}
}

#endif
