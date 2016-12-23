#include "alert.h"

using namespace v8;

Nan::Persistent<Function> AlertWrap::constructor;

NAN_MODULE_INIT(AlertWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("AlertWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "what", what);
  Nan::SetPrototypeMethod(tpl, "type", type);
  Nan::SetPrototypeMethod(tpl, "message", message);
  Nan::SetPrototypeMethod(tpl, "category", category);
  Nan::SetPrototypeMethod(tpl, "handle", handle);
  Nan::SetPrototypeMethod(tpl, "error", error);
  Nan::SetPrototypeMethod(tpl, "params", params);
  Nan::SetPrototypeMethod(tpl, "infoHash", info_hash);


  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("AlertWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> AlertWrap::New(const libtorrent::alert* a) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    if(a) {
        Nan::ObjectWrap::Unwrap<AlertWrap>(obj.ToLocalChecked())->alert_ = a;
    } else {
      obj = Nan::New<Object>();
    }
    return scope.Escape(obj.ToLocalChecked());
};

AlertWrap::AlertWrap() {
  alert_ = NULL;
};

AlertWrap::~AlertWrap() {
  if (alert_ != NULL)
    delete alert_;
};

NAN_METHOD(AlertWrap::NewInstance) {
  Nan::HandleScope scope;

  AlertWrap* obj = new AlertWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};

NAN_METHOD(AlertWrap::what) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<String>(AlertWrap::Unwrap(info.This())->what()).ToLocalChecked());
};

NAN_METHOD(AlertWrap::type) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<Integer>(AlertWrap::Unwrap(info.This())->type()));
};

NAN_METHOD(AlertWrap::message) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<String>(AlertWrap::Unwrap(info.This())->message()).ToLocalChecked());
};

NAN_METHOD(AlertWrap::category) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<Integer>(AlertWrap::Unwrap(info.This())->category()));
};

NAN_METHOD(AlertWrap::handle) {
    Nan::HandleScope scope;

    const libtorrent::alert* a = AlertWrap::Unwrap(info.This());

    auto casted = dynamic_cast<const libtorrent::torrent_alert*>(a);

    if (!casted) {
      info.GetReturnValue().SetUndefined();
    } else {
      info.GetReturnValue().Set(TorrentHandleWrap::New(casted->handle));
    }
};

NAN_METHOD(AlertWrap::error) {
    Nan::HandleScope scope;

    const libtorrent::alert* a = AlertWrap::Unwrap(info.This());

    auto casted = dynamic_cast<const libtorrent::add_torrent_alert*>(a);

    if (!casted) {
      info.GetReturnValue().SetUndefined();
    } else {
      info.GetReturnValue().Set(Nan::New<Integer>(casted->error.value()));
    }
};

NAN_METHOD(AlertWrap::params) {
    Nan::HandleScope scope;

    const libtorrent::alert* a = AlertWrap::Unwrap(info.This());

    auto casted = dynamic_cast<const libtorrent::add_torrent_alert*>(a);

    if (!casted) {
      info.GetReturnValue().SetUndefined();
    } else {
      info.GetReturnValue().Set(AddTorrentParamsWrap::New(&casted->params));
    }
};

NAN_METHOD(AlertWrap::info_hash) {
    Nan::HandleScope scope;

    const libtorrent::alert* a = AlertWrap::Unwrap(info.This());

    auto casted = dynamic_cast<const libtorrent::dht_get_peers_reply_alert*>(a);

    if (!casted) {
      info.GetReturnValue().SetUndefined();
    } else {
      info.GetReturnValue().Set(Nan::New<String>(libtorrent::to_hex(casted->info_hash.to_string())).ToLocalChecked());
    }
};
