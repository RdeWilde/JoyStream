#include "session.h"

// Wrapper Impl

Nan::Persistent<v8::Function> SessionWrap::constructor;

NAN_MODULE_INIT(SessionWrap::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Session").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "addTorrent", add_torrent);
  Nan::SetPrototypeMethod(tpl, "removeTorrent", remove_torrent);
  Nan::SetPrototypeMethod(tpl, "listenPort", listen_port);
  Nan::SetPrototypeMethod(tpl, "postTorrentUpdates", post_torrent_updates);
  Nan::SetPrototypeMethod(tpl, "pause", pause);
  Nan::SetPrototypeMethod(tpl, "isPaused", is_paused);
  Nan::SetPrototypeMethod(tpl, "resume", resume);
  Nan::SetPrototypeMethod(tpl, "findTorrent", find_torrent);
  Nan::SetPrototypeMethod(tpl, "popAlerts", pop_alerts);
  Nan::SetPrototypeMethod(tpl, "setAlertNotify", set_alert_notify);
  Nan::SetPrototypeMethod(tpl, "dhtAnnounce", dht_announce);
  Nan::SetPrototypeMethod(tpl, "dhtGetPeers", dht_get_peers);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Session").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

libtorrent::sha1_hash SessionWrap::object_to_sha1_hash(v8::Local<v8::Value> infoHash) {
  v8::String::Utf8Value s(infoHash);
  std::string info_hash = std::string(*s);

  if(info_hash.size() != 40) {
    Nan::ThrowTypeError("incorrent length of hex string");
  }

  char buf[21];

  if(!libtorrent::from_hex(info_hash.c_str(), info_hash.size(), buf)) {
    Nan::ThrowTypeError("invalid hex string");
  }

  return libtorrent::sha1_hash(buf);
}

NAN_METHOD(SessionWrap::New) {
  if (info.IsConstructCall()) {
    SessionWrap *obj = new SessionWrap();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

NAN_METHOD(SessionWrap::add_torrent) {
  if (info.Length() < 7) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  libtorrent::add_torrent_params params;

  unsigned int uploadLimit = info[0]->Uint32Value();
  unsigned int downloadLimit = info[1]->Uint32Value();
  v8::String::Utf8Value s1(info[2]);
  std::string name = std::string(*s1);
  std::vector<char> resumeData;
  v8::String::Utf8Value s2(info[4]);
  std::string savePath = std::string(*s2);

  params.upload_limit = uploadLimit;
  params.download_limit = downloadLimit;
  params.name = name;
  params.resume_data = resumeData;
  params.save_path = savePath;
  params.flags &= ~libtorrent::add_torrent_params::flags_t::flag_paused;
  params.flags &= ~libtorrent::add_torrent_params::flags_t::flag_auto_managed;
  params.flags |= libtorrent::add_torrent_params::flag_duplicate_is_error;
  params.info_hash = object_to_sha1_hash(info[5]);

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  Nan::Callback *callback = new Nan::Callback(info[6].As<v8::Function>());
  session_wrap->session_.plugin_->submit(joystream::extension::request::AddTorrent(params, [callback](libtorrent::error_code ec, libtorrent::torrent_handle th){

    if(ec) {
        v8::Local<v8::Value> argv[] = { Nan::New(ec.value()) };

        callback->Call(1, argv);
        return;
    }

    v8::Local<v8::Value> argv[] = {
        Nan::Null(),
        TorrentHandleWrap::New(th)
    };

    callback->Call(2, argv);

  }));
}

NAN_METHOD(SessionWrap::remove_torrent) {
  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  libtorrent::sha1_hash info_hash = object_to_sha1_hash(info[0]);

  Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
  session_wrap->session_.plugin_->submit(joystream::extension::request::RemoveTorrent(info_hash, [callback](const std::exception_ptr &ex){

    v8::Local<v8::Value> argv[] = {
        Nan::Null()
    };

    if(ex) {
        argv[0] = Nan::New<v8::String>("Missing Torrent").ToLocalChecked();
    }

    callback->Call(1, argv);
  }));
}

NAN_METHOD(SessionWrap::listen_port) {
  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  info.GetReturnValue().Set(session_wrap->session_.s->listen_port());
}

NAN_METHOD(SessionWrap::post_torrent_updates) {
  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  session_wrap->session_.s->post_torrent_updates();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(SessionWrap::pause) {
  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  // Pause libtorrent session
  Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
  session_wrap->session_.plugin_->submit(joystream::extension::request::PauseLibtorrent([session_wrap, callback]() {

    std::clog << "Libtorrent session paused" << std::endl;

    // Stop all plugins
    session_wrap->session_.plugin_->submit(joystream::extension::request::StopAllTorrentPlugins([callback]() {

        std::clog << "All plugins stopped" << std::endl;

        // Service user callback
        v8::Local<v8::Value> argv[] = {};

        callback->Call(0, argv);
    }));
  }));
  session_wrap->session_.s->pause();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(SessionWrap::is_paused) {

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());
  v8::Local<v8::Boolean> isPaused;
  if (session_wrap->session_.s->is_paused()) {
    isPaused = Nan::True();
  } else {
    isPaused = Nan::False();
  }
  info.GetReturnValue().Set(isPaused);
}

NAN_METHOD(SessionWrap::resume) {
  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());
  session_wrap->session_.s->resume();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(SessionWrap::find_torrent) {
  libtorrent::torrent_handle th;

  libtorrent::sha1_hash info_hash;
  info_hash = object_to_sha1_hash(info[0]);

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());
  th = session_wrap->session_.s->find_torrent(info_hash);

  info.GetReturnValue().Set(TorrentHandleWrap::New(th));
}

NAN_METHOD(SessionWrap::pop_alerts) {
  v8::Local<v8::Array> ret = Nan::New<v8::Array>();

  std::vector<libtorrent::alert*> alerts;

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  session_wrap->session_.s->pop_alerts(&alerts);

  for(const libtorrent::alert * alert : alerts)
    ret->Set(ret->Length(), AlertWrap::New(alert));

  info.GetReturnValue().Set(ret);
}

NAN_METHOD(SessionWrap::set_alert_notify) {

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  /*
   * Need to define the logic here
   */

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(SessionWrap::dht_announce) {
  libtorrent::sha1_hash info_hash;
  info_hash = object_to_sha1_hash(info[0]);

  unsigned int listen_port = info[1]->Uint32Value();

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  session_wrap->session_.s->dht_announce(info_hash, listen_port);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(SessionWrap::dht_get_peers) {
  libtorrent::sha1_hash info_hash;
  info_hash = object_to_sha1_hash(info[0]);

  unsigned int listen_port = info[1]->Uint32Value();

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  session_wrap->session_.s->dht_announce(info_hash, listen_port);

  info.GetReturnValue().Set(Nan::Undefined());
}
