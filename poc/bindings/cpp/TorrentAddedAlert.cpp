/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include  "TorrentAddedAlert.hpp"

namespace joystream {
namespace node_addon {

NAN_MODULE_INIT(TorrentAddedAlert::Init) {

  v8::Local<v8::FunctionTemplate> t = Nan::New<v8::FunctionTemplate>(New);

  TorrentAlert::Init("TorrentAddedAlert", t);

  constructor.Reset(Nan::GetFunction(t).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentAddedAlert").ToLocalChecked(), Nan::GetFunction(t).ToLocalChecked());
}

v8::Local<v8::Object> TorrentAddedAlert::NewInstance(const libtorrent::torrent_added_alert * alert) {

  /**
  Nan::EscapableHandleScope scope;

  Local<Function> cons = Nan::New(constructor);
  Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());
  Nan::ObjectWrap::Unwrap<TorrentStatusWrap>(obj.ToLocalChecked())->torrent_status_ = ts;

  return scope.Escape(obj.ToLocalChecked());
  */
}

NAN_METHOD(TorrentAddedAlert::New) {

  NEW_OPERATOR_GUARD(info, constructor)

  libtorrent::torrent_handle handle;

  if(info.Length() == 0);
    // we allow this case
  else if(info.Length() == 1) {

    TorrentHandleWrap * wrapper = Nan::ObjectWrap::Unwrap<TorrentHandleWrap>(info[0]);

    if(wrapper)
      (new TorrentAddedAlert(wrapper->th_))->Wrap(info.This());
    else
      Nan::ThrowError("Invalid argument type: must be a TorrentHandle");

  } else // Is this a reasonable approach?
    Nan::ThrowError("Invalid number of arguments: 1 or 0 required");
  
  info.GetReturnValue().Set(info.This());
}

TorrentAddedAlert::TorrentAddedAlert(const libtorrent::torrent_handle & handle)
  : TorrentAlert(handle) {
}

std::string TorrentAddedAlert::message() const {
  return TorrentAlert::message() + " added";
}

}
}
