/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include "Alert.hpp"

namespace joystream {
namespace node_addon {

void Alert::Init(const std::string & className,
                 v8::Local<v8::FunctionTemplate> & t,
                 int internalFieldCount) {

    Nan::SetPrototypeMethod(t, "type", type);
    Nan::SetPrototypeMethod(t, "what", what);
    Nan::SetPrototypeMethod(t, "message", message);
    Nan::SetPrototypeMethod(t, "category", category);

    t->SetClassName(Nan::New(className.c_str()).ToLocalChecked());
    t->InstanceTemplate()->SetInternalFieldCount(internalFieldCount);
}

NAN_METHOD(Alert::type) {
    const Alert * a = Nan::ObjectWrap::Unwrap<Alert>(info.This());
    info.GetReturnValue().Set(Nan::New(a->type()));
}

NAN_METHOD(Alert::what) {
    const Alert * a = Nan::ObjectWrap::Unwrap<Alert>(info.This());
    info.GetReturnValue().Set(Nan::New(a->what()).ToLocalChecked());
}

NAN_METHOD(Alert::message) {
    const Alert * a = Nan::ObjectWrap::Unwrap<Alert>(info.This());
    info.GetReturnValue().Set(Nan::New(a->message().c_str()).ToLocalChecked());
}

NAN_METHOD(Alert::category) {
    const Alert * a = Nan::ObjectWrap::Unwrap<Alert>(info.This());
    info.GetReturnValue().Set(Nan::New(a->category()));
}

}
}
