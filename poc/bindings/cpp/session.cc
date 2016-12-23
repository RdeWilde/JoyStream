#include "session.h"

// Wrapper Impl

Nan::Persistent<v8::Function> SessionWrap::constructor;

NAN_MODULE_INIT(SessionWrap::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("SessionWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "addTorrent", AddTorrent);
  Nan::SetPrototypeMethod(tpl, "removeTorrent", RemoveTorrent);
  Nan::SetPrototypeMethod(tpl, "listenPort", ListenPort);
  Nan::SetPrototypeMethod(tpl, "postTorrentUpdates", PostTorrentUpdates);
  Nan::SetPrototypeMethod(tpl, "pause", Pause);
  Nan::SetPrototypeMethod(tpl, "isPaused", IsPaused);
  Nan::SetPrototypeMethod(tpl, "resume", Resume);
  Nan::SetPrototypeMethod(tpl, "findTorrent", FindTorrent);
  Nan::SetPrototypeMethod(tpl, "popAlerts", PopAlerts);
  Nan::SetPrototypeMethod(tpl, "setAlertNotify", SetAlertNotify);

  Nan::SetPrototypeMethod(tpl, "dhtAnnounce", dht_announce);
  Nan::SetPrototypeMethod(tpl, "dhtGetPeers", dht_get_peers);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("SessionWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

SessionWrap::SessionWrap() {
}

SessionWrap::~SessionWrap() {
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

NAN_METHOD(SessionWrap::AddTorrent) {
  Nan::HandleScope scope;

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
  v8::String::Utf8Value s3(info[5]);
  std::string info_hash = std::string(*s3);

  if(info_hash.size() != 40) {
    Nan::ThrowTypeError("incorrent length of hex string");
    return;
  }

  char buf[21];

  if(!libtorrent::from_hex(info_hash.c_str(), info_hash.size(), buf)) {
    Nan::ThrowTypeError("invalid hex string");
    return;
  }

  params.upload_limit = uploadLimit;
  params.download_limit = downloadLimit;
  params.name = name;
  params.resume_data = resumeData;
  params.save_path = savePath;
  params.flags &= ~libtorrent::add_torrent_params::flags_t::flag_paused;
  params.flags &= ~libtorrent::add_torrent_params::flags_t::flag_auto_managed;
  params.flags |= libtorrent::add_torrent_params::flag_duplicate_is_error;
  params.info_hash = libtorrent::sha1_hash(buf);


  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  Nan::Callback *callback = new Nan::Callback(info[6].As<v8::Function>());
  session_wrap->session_.plugin_->submit(joystream::extension::request::AddTorrent(params, [callback](libtorrent::error_code ec, libtorrent::torrent_handle th){
    printf("Torrent added !!!! \n");

    v8::Local<v8::Value> argv[] = {
        Nan::Null()
      , Nan::New<v8::Number>(1)
    };

    callback->Call(2, argv);
  }));
}

NAN_METHOD(SessionWrap::RemoveTorrent) {
  Nan::HandleScope scope;

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  /*
    Remove torrent logic here
  */

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(SessionWrap::ListenPort) {
  Nan::HandleScope scope;

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  info.GetReturnValue().Set(session_wrap->session_.s->listen_port());
}

NAN_METHOD(SessionWrap::PostTorrentUpdates) {
  Nan::HandleScope scope;

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  session_wrap->session_.s->post_torrent_updates();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(SessionWrap::Pause) {
  Nan::HandleScope scope;

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());
  session_wrap->session_.s->pause();
  printf("Plugin paused \n");
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(SessionWrap::IsPaused) {
  Nan::HandleScope scope;

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());
  v8::Local<v8::Boolean> isPaused;
  if (session_wrap->session_.s->is_paused()) {
    isPaused = Nan::True();
  } else {
    isPaused = Nan::False();
  }
  info.GetReturnValue().Set(isPaused);
}

NAN_METHOD(SessionWrap::Resume) {
  Nan::HandleScope scope;

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());
  session_wrap->session_.s->resume();
  printf("Plugin resumed \n");
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(SessionWrap::FindTorrent) {
  Nan::HandleScope scope;
  libtorrent::torrent_handle th;

  libtorrent::sha1_hash info_hash;
  info_hash = object_to_sha1_hash(info[0]);

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());
  th = session_wrap->session_.s->find_torrent(info_hash);

  /*
   * Not finished need to define TorrentHandleWrap
   */

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(SessionWrap::PopAlerts) {
  Nan::HandleScope scope;

  v8::Local<v8::Array> ret = Nan::New<v8::Array>();

  std::vector<libtorrent::alert*> alerts;

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  session_wrap->session_.s->pop_alerts(&alerts);

  for(const libtorrent::alert * alert : alerts)
    ret->Set(ret->Length(), AlertWrap::New(alert));

  info.GetReturnValue().Set(ret);
}

NAN_METHOD(SessionWrap::SetAlertNotify) {
  Nan::HandleScope scope;

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  /*
   * Need to define the logic here
   */

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(SessionWrap::dht_announce) {
  Nan::HandleScope scope;

  libtorrent::sha1_hash info_hash;
  info_hash = object_to_sha1_hash(info[0]);

  unsigned int listen_port = info[1]->Uint32Value();

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  session_wrap->session_.s->dht_announce(info_hash, listen_port);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(SessionWrap::dht_get_peers) {
  Nan::HandleScope scope;

  libtorrent::sha1_hash info_hash;
  info_hash = object_to_sha1_hash(info[0]);

  unsigned int listen_port = info[1]->Uint32Value();

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  session_wrap->session_.s->dht_announce(info_hash, listen_port);

  info.GetReturnValue().Set(Nan::Undefined());
}
