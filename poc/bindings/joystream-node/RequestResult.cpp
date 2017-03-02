/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 25 2017
 */

#include "RequestResult.hpp"
#include "libtorrent-node/utils.hpp"

#define UNWRAP_THIS(var) RequestResult * var = Nan::ObjectWrap::Unwrap<RequestResult>(info.This());

namespace joystream {
namespace node {

 Nan::Persistent<v8::Function> RequestResult::constructor;

 NAN_MODULE_INIT(RequestResult::Init) {

   // Create constructor function
   v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
   //tpl->SetClassName(Nan::New("RequestResult").ToLocalChecked());

   v8::Local<v8::ObjectTemplate> itpl = tpl->InstanceTemplate();
   itpl->SetInternalFieldCount(1);

   // Accessors are defined on instance
   Nan::SetAccessor(itpl, Nan::New("type").ToLocalChecked(), Type);

   // Methods on prototype
   Nan::SetPrototypeMethod(tpl, "run", Run);

   Nan::SetPrototypeMethod(tpl, "what", What);
   Nan::SetPrototypeMethod(tpl, "message", Message);
   Nan::SetPrototypeMethod(tpl, "category", Category);

   constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
   //Nan::Set(target, Nan::New("RequestResult").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
 }

 v8::Local<v8::Object> RequestResult::NewInstance(const extension::alert::RequestResult * alert) {

   // Create object using constructor
   NEW_OBJECT(constructor, o)

   // Set callback on wrapped object
   RequestResult * requestResult = Nan::ObjectWrap::Unwrap<RequestResult>(o);

   requestResult->_loadedCallback = alert->loadedCallback;
   requestResult->_type = alert->type();
   requestResult->_what = alert->what();
   requestResult->_message = alert->message();
   requestResult->_category = alert->category();

   return o;
 }

 NAN_METHOD(RequestResult::New) {

   NEW_OPERATOR_GUARD(info, constructor)

   (new RequestResult())->Wrap(info.This());

   RETURN(info.This())
 }

 NAN_METHOD(RequestResult::Run) {

   UNWRAP_THIS(requestResult)
   requestResult->_loadedCallback();
   RETURN_VOID
 }

NAN_GETTER(RequestResult::Type) {

   RequestResult * requestResult = Nan::ObjectWrap::Unwrap<RequestResult>(info.Holder());

   v8::Local<v8::Number> type = Nan::New(requestResult->_type);

   RETURN(type)
}

 NAN_METHOD(RequestResult::What) {

   UNWRAP_THIS(requestResult)
   Nan::MaybeLocal<v8::String> what = Nan::New(requestResult->_what);
   RETURN(what.ToLocalChecked())
 }

 NAN_METHOD(RequestResult::Message) {

   UNWRAP_THIS(requestResult)
   Nan::MaybeLocal<v8::String> message = Nan::New(requestResult->_message);
   RETURN(message.ToLocalChecked())
 }

 NAN_METHOD(RequestResult::Category) {

   UNWRAP_THIS(requestResult)
   v8::Local<v8::Number> category = Nan::New(requestResult->_category);
   RETURN(category)
 }

}
}
