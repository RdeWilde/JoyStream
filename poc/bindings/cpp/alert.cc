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
  Nan::SetPrototypeMethod(tpl, "status", status);
  Nan::SetPrototypeMethod(tpl, "peers", peers);
  Nan::SetPrototypeMethod(tpl, "endpoint", endpoint);
  Nan::SetPrototypeMethod(tpl, "ip", ip);
  Nan::SetPrototypeMethod(tpl, "loadedCallback", loaded_callback);


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

/*AlertWrap::AlertWrap() {
  //alert_ = NULL;
};*/

/*AlertWrap::~AlertWrap() {
  std::cout << "Destroy alert" << std::endl;
  std::cout << alert_->type() << std::endl;
  //if (alert_ != NULL)
  //  delete alert_;
};*/

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
      info.GetReturnValue().Set(AddTorrentParamsWrap::New(casted->params));
    }
};

NAN_METHOD(AlertWrap::info_hash) {
    Nan::HandleScope scope;

    const libtorrent::alert* a = AlertWrap::Unwrap(info.This());

    // Not optimal. Can be improved.
    switch (a->type()) {
      case 87: {
          auto casted = dynamic_cast<const libtorrent::dht_get_peers_reply_alert*>(a);
          info.GetReturnValue().Set(Nan::New<String>(libtorrent::to_hex(casted->info_hash.to_string())).ToLocalChecked());
        }
        break;
      case 4: {
          auto casted = dynamic_cast<const libtorrent::torrent_removed_alert*>(a);
          info.GetReturnValue().Set(Nan::New<String>(libtorrent::to_hex(casted->info_hash.to_string())).ToLocalChecked());
        }
        break;
      default:
        info.GetReturnValue().SetUndefined();
    }

};

NAN_METHOD(AlertWrap::status) {
    Nan::HandleScope scope;

    v8::Local<v8::Array> ret = Nan::New<v8::Array>();

    const libtorrent::alert* a = AlertWrap::Unwrap(info.This());

    auto casted = dynamic_cast<const libtorrent::state_update_alert*>(a);

    if (!casted) {
      info.GetReturnValue().SetUndefined();
    } else {
      for(const libtorrent::torrent_status ts : casted->status)
        ret->Set(ret->Length(), TorrentStatusWrap::New(ts));
      info.GetReturnValue().Set(ret);
    }

};

NAN_METHOD(AlertWrap::peers) {
    Nan::HandleScope scope;

    v8::Local<v8::Array> ret = Nan::New<v8::Array>();

    const libtorrent::alert* a = AlertWrap::Unwrap(info.This());

    auto casted = dynamic_cast<const libtorrent::dht_get_peers_reply_alert*>(a);

    if (!casted) {
      info.GetReturnValue().SetUndefined();
    } else {
      for(const libtorrent::tcp::endpoint ep : casted->peers())
        ret->Set(ret->Length(), EndpointWrap::New(ep));
      info.GetReturnValue().Set(ret);
    }
};

NAN_METHOD(AlertWrap::endpoint) {
    Nan::HandleScope scope;

    const libtorrent::alert* a = AlertWrap::Unwrap(info.This());

    auto casted = dynamic_cast<const libtorrent::listen_succeeded_alert*>(a);

    if (!casted) {
      info.GetReturnValue().SetUndefined();
    } else {
      info.GetReturnValue().Set(EndpointWrap::New(casted->endpoint));
    }
};

NAN_METHOD(AlertWrap::ip) {
    Nan::HandleScope scope;

    const libtorrent::alert* a = AlertWrap::Unwrap(info.This());

    auto casted = dynamic_cast<const libtorrent::peer_connect_alert*>(a);

    if (!casted) {
      info.GetReturnValue().SetUndefined();
    } else {
      info.GetReturnValue().Set(EndpointWrap::New(casted->ip));
    }
};

NAN_METHOD(AlertWrap::loaded_callback) {
    Nan::HandleScope scope;

    const libtorrent::alert* a = AlertWrap::Unwrap(info.This());

    auto casted = dynamic_cast<const joystream::extension::alert::RequestResult*>(a);

    if (casted) {
      casted->loadedCallback();
    } else {
      Nan::ThrowTypeError("Not a RequestResult alert");
    }
    info.GetReturnValue().SetUndefined();
};
