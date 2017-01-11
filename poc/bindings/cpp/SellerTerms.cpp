/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 11 2017
 */

#include "SellerTerms.hpp"
#include "util.hpp"

namespace joystream {
namespace node_addon {

Nan::Persistent<v8::Function> SellerTerms::constructor;

NAN_MODULE_INIT(SellerTerms::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("SellerTerms").ToLocalChecked());

  v8::Local<v8::ObjectTemplate> t = tpl->InstanceTemplate();
  t->SetInternalFieldCount(1);

  Nan::SetAccessor(t, Nan::New("minPrice").ToLocalChecked(), SellerTerms::minPrice);
  Nan::SetAccessor(t, Nan::New("minLock").ToLocalChecked(), SellerTerms::minLock);
  Nan::SetAccessor(t, Nan::New("maxSellers").ToLocalChecked(), SellerTerms::maxSellers);
  Nan::SetAccessor(t, Nan::New("minContractFeePerKb").ToLocalChecked(), SellerTerms::minContractFeePerKb);
  Nan::SetAccessor(t, Nan::New("settlementFee").ToLocalChecked(), SellerTerms::settlementFee);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("SellerTerms").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

v8::Local<v8::Object> SellerTerms::NewInstance(const protocol_wire::SellerTerms & terms) {
    Nan::EscapableHandleScope scope;

    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto instance = cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked();

    ObjectWrap::Unwrap<SellerTerms>(instance)->_terms = terms;
    return scope.Escape(instance);
}

SellerTerms::SellerTerms(const protocol_wire::SellerTerms & terms)
  : _terms(terms) {
}

NAN_METHOD(SellerTerms::New) {

  NEW_OPERATOR_GUARD(info, constructor)

  protocol_wire::SellerTerms terms;

  if(info.Length() == 0);
    // we allow this case
  else if(info.Length() == 5) {

    // I was unable to find any proper documentation on what
    // is returned if we dont actally have integers here?
    // we would want to catch any implicit conversion and

    // Desired type: quint64
    terms.setMinPrice(info[0]->IntegerValue());

    // Desired type: uint16_t
    terms.setMinLock(info[1]->IntegerValue());

    // Desired type: quint32
    terms.setMaxSellers(info[2]->IntegerValue());

    // Desired type: quint64
    terms.setMinContractFeePerKb(info[3]->IntegerValue());

    // Desired type: quint64
    terms.setSettlementFee(info[4]->IntegerValue());

  } else // Is this a reasonable approach?
    Nan::ThrowError("Invalid number of arguments: 5 or 0 required");

  // Create
  (new SellerTerms(terms))->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

NAN_GETTER(SellerTerms::minPrice) {
    auto data = ObjectWrap::Unwrap<SellerTerms>(info.This())->_terms.minPrice();
    info.GetReturnValue().Set(Nan::New<v8::Number>(data));
}

NAN_GETTER(SellerTerms::minLock) {
  auto data = ObjectWrap::Unwrap<SellerTerms>(info.This())->_terms.minLock();
  info.GetReturnValue().Set(Nan::New<v8::Number>(data));
}

NAN_GETTER(SellerTerms::maxSellers) {
  auto data = ObjectWrap::Unwrap<SellerTerms>(info.This())->_terms.maxSellers();
  info.GetReturnValue().Set(Nan::New<v8::Number>(data));
}

NAN_GETTER(SellerTerms::minContractFeePerKb) {
  auto data = ObjectWrap::Unwrap<SellerTerms>(info.This())->_terms.minContractFeePerKb();
  info.GetReturnValue().Set(Nan::New<v8::Number>(data));
}

NAN_GETTER(SellerTerms::settlementFee) {
  auto data = ObjectWrap::Unwrap<SellerTerms>(info.This())->_terms.settlementFee();
  info.GetReturnValue().Set(Nan::New<v8::Number>(data));
}

}
}
