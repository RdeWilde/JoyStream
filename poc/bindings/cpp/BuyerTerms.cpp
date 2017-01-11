/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 9 2017
 */

#include "BuyerTerms.hpp"
#include "util.hpp"

namespace joystream {
namespace node_addon {

Nan::Persistent<v8::Function> BuyerTerms::constructor;

NAN_MODULE_INIT(BuyerTerms::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("BuyerTerms").ToLocalChecked());

  v8::Local<v8::ObjectTemplate> t = tpl->InstanceTemplate();
  t->SetInternalFieldCount(1);

  Nan::SetAccessor(t, Nan::New("maxPrice").ToLocalChecked(), BuyerTerms::maxPrice);
  Nan::SetAccessor(t, Nan::New("maxLock").ToLocalChecked(), BuyerTerms::maxLock);
  Nan::SetAccessor(t, Nan::New("minNumberOfSellers").ToLocalChecked(), BuyerTerms::minNumberOfSellers);
  Nan::SetAccessor(t, Nan::New("maxContractFeePerKb").ToLocalChecked(), BuyerTerms::maxContractFeePerKb);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("BuyerTerms").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

v8::Local<v8::Object> BuyerTerms::NewInstance(const protocol_wire::BuyerTerms & terms) {
    Nan::EscapableHandleScope scope;

    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto instance = cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked();
    
    ObjectWrap::Unwrap<BuyerTerms>(instance)->_terms = terms;
    return scope.Escape(instance);
}

BuyerTerms::BuyerTerms(const protocol_wire::BuyerTerms & terms)
  : _terms(terms) {
}

NAN_METHOD(BuyerTerms::New) {

  NEW_OPERATOR_GUARD(info, constructor)

  protocol_wire::BuyerTerms terms;

  if(info.Length() == 0);
    // we allow this case
  else if(info.Length() == 4) {

    // I was unable to find any proper documentation on what
    // is returned if we dont actally have integers here?
    // we would want to catch any implicit conversion and

    // Desired type: quint64
    terms.setMaxPrice(info[0]->IntegerValue());

    // Desired type: uint16_t
    terms.setMaxLock(info[1]->IntegerValue());

    // Desired type: quint32
    terms.setMinNumberOfSellers(info[2]->IntegerValue());

    // Desired type: quint64
    terms.setMaxContractFeePerKb(info[3]->IntegerValue());

  } else // Is this a reasonable approach?
    Nan::ThrowError("Invalid number of arguments: 4 or 0 required");

  // Create
  (new BuyerTerms(terms))->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

NAN_GETTER(BuyerTerms::maxPrice) {
    auto data = ObjectWrap::Unwrap<BuyerTerms>(info.This())->_terms.maxPrice();
    info.GetReturnValue().Set(Nan::New<v8::Number>(data));
}

NAN_GETTER(BuyerTerms::maxLock) {
  auto data = ObjectWrap::Unwrap<BuyerTerms>(info.This())->_terms.maxLock();
  info.GetReturnValue().Set(Nan::New<v8::Number>(data));
}

NAN_GETTER(BuyerTerms::minNumberOfSellers) {
  auto data = ObjectWrap::Unwrap<BuyerTerms>(info.This())->_terms.minNumberOfSellers();
  info.GetReturnValue().Set(Nan::New<v8::Number>(data));
}

NAN_GETTER(BuyerTerms::maxContractFeePerKb) {
  auto data = ObjectWrap::Unwrap<BuyerTerms>(info.This())->_terms.maxContractFeePerKb();
  info.GetReturnValue().Set(Nan::New<v8::Number>(data));
}

}
}
