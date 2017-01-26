#include "session.h"
#include "plugin.hpp"

namespace libtorrent {
namespace node {


Nan::Persistent<v8::Function> SessionWrap::constructor;

Nan::Callback SessionWrap::_alertNotifier;

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

  NEW_OPERATOR_GUARD(info, constructor)

  (new SessionWrap())->Wrap(info.This());

  RETURN(info.This())
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
        TorrentHandle::New(th)
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

  std::shared_ptr<Nan::Callback> callback;

  if(info.Length() > 0) {
      callback.reset(new Nan::Callback(info[0].As<v8::Function>()));
  }

  // Pause libtorrent session
  session_wrap->session_.plugin_->submit(joystream::extension::request::PauseLibtorrent([session_wrap, callback]() {

    std::clog << "Libtorrent session paused" << std::endl;

    // Stop all plugins
    session_wrap->session_.plugin_->submit(joystream::extension::request::StopAllTorrentPlugins([callback]() {

        std::clog << "All plugins stopped" << std::endl;

        // Service user callback
        if(callback && !callback->IsEmpty()) {
            callback->Call(0, {});
        }
    }));
  }));

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

  info.GetReturnValue().Set(TorrentHandle::New(th));
}

#ifndef TORRENT_DISABLE_EXTENSIONS
NAN_METHOD(SessionWrap::add_extension) {

  // Recover the plugin binding
  // ***** USER MUST SUPPLY WRAPPED OBJECT OF CORRECT KIND, OR V8 DIES *****
  EXPLOSIVE_ARGUMENT_REQUIRE_WRAPS(0, libtorrent::node::plugin, p)

  // Recover session binding
  SessionWrap * session = Nan::ObjectWrap::Unwrap<SessionWrap>(info.This());

  // Add underlying plugin to underlying session
  session->session_.s->add_extension(p->getPlugin());

  // Get alert converter for plugin, and add it to list of converters.
  session->_decoders.push_back(p->getDecoder());
}
#endif // TORRENT_DISABLE_EXTENSIONS

NAN_METHOD(SessionWrap::pop_alerts) {

  // Recover session binding
  SessionWrap * session = Nan::ObjectWrap::Unwrap<SessionWrap>(info.This());

  // Get currently pending alerts from libtorrent
  std::vector<libtorrent::alert*> alerts;
  session->session_.s->pop_alerts(&alerts);

  // Iterate alerts, and convert to js objects
  v8::Local<v8::Array> ret = Nan::New<v8::Array>();
  for(const libtorrent::alert * alert : alerts) {

    // Iterate decoders to find match
    for(AlertDecoder decoder : session->_decoders) {

      // decode
      auto v = decoder(alert);

      // if decoded, then store, and break to next alert
      if(v.is_initialized()) {
        ret->Set(ret->Length(), v.get());
        break;
      }

    }
  }

  info.GetReturnValue().Set(ret);
}

NAN_METHOD(SessionWrap::set_alert_notify) {

  ARGUMENTS_REQUIRE_FUNCTION(0, fn);

  // Recover session binding
  SessionWrap* session_wrap = Nan::ObjectWrap::Unwrap<SessionWrap>(info.This());

  // Store persistent handle to callback
  _alertNotifier.Reset(fn);

  // Set alert notifier on libtorrent session
  session_wrap->session_.s->set_alert_notify([]() { _alertNotifier(0, {}); });
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

// Alert converter for (subset) of built in libtorrent alerts
boost::optional<v8::Local<v8::Object>> SessionWrap::DefaultAlertDecoder(const libtorrent::alert *) {

  // Return value
  boost::optional<v8::Local<v8::Object>> v;

  /**
  // TODO
  if(libtorrent::metadata_received_alert const * p = libtorrent::alert_cast<libtorrent::metadata_received_alert>(a))
      v = toObject(p);
  else if(libtorrent::metadata_failed_alert const * p = libtorrent::alert_cast<libtorrent::metadata_failed_alert>(a))
      v = toObject(p);
  else if(libtorrent::listen_succeeded_alert const * p = libtorrent::alert_cast<libtorrent::listen_succeeded_alert>(a))
      v = toObject(p);
  else if(libtorrent::add_torrent_alert const * p = libtorrent::alert_cast<libtorrent::add_torrent_alert>(a))
      v = toObject(p);
  else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a))
      v = toObject(p);
  else if (libtorrent::torrent_paused_alert const * p = libtorrent::alert_cast<libtorrent::torrent_paused_alert>(a))
      v = toObject(p);
  else if (libtorrent::state_update_alert const * p = libtorrent::alert_cast<libtorrent::state_update_alert>(a))
      v = toObject(p);
  else if(libtorrent::save_resume_data_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_alert>(a))
      v = toObject(p);
  else if(libtorrent::save_resume_data_failed_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_failed_alert>(a))
      v = toObject(p);
  else if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a))
      v = toObject(p);
  else if(libtorrent::torrent_checked_alert const * p = libtorrent::alert_cast<libtorrent::torrent_checked_alert>(a))
      v = toObject(p);
  else if(libtorrent::read_piece_alert const * p = libtorrent::alert_cast<libtorrent::read_piece_alert>(a))
      v = toObject(p);
  else if(libtorrent::piece_finished_alert const * p = libtorrent::alert_cast<libtorrent::piece_finished_alert>(a))
      v = toObject(p);
  //else
      // ..
  */

  return v;

}

}
}
