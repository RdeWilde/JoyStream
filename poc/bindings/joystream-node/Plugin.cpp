/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include "Plugin.hpp"
#include "PluginAlertEncoder.hpp"
#include "libtorrent-node/utils.hpp"
#include "libtorrent-node/sha1_hash.hpp"

#include <extension/extension.hpp>

namespace detail {

  joystream::extension::request::SubroutineHandler CreateGenericSubroutineHandler(const std::shared_ptr<Nan::Callback> & callback);

}

/// Plugin utilities
#define ARGUMENTS_REQUIRE_CALLBACK(i, var)                              \
  ARGUMENTS_REQUIRE_FUNCTION(i, _fn)                                           \
  std::shared_ptr<Nan::Callback> var = std::make_shared<Nan::Callback>(_fn);

#define GET_THIS_PLUGIN(var) Plugin * var = Nan::ObjectWrap::Unwrap<Plugin>(info.This());

/// Returned values

#define TERNARY_CAST(x) ((Nan::To<v8::Object>(x)).ToLocalChecked())

#define RESULT_VALUE_SUCCESS Nan::True()
#define RESULT_VALUE_FAILURE Nan::Undefined()
#define RESULT_VALUE(success) (success ? TERNARY_CAST(RESULT_VALUE_SUCCESS) : TERNARY_CAST(RESULT_VALUE_FAILURE))

#define ERROR_VALUE_SUCCESS Nan::Null()
#define ERROR_VALUE_FAILURE(str) (Nan::New(str).ToLocalChecked())
#define ERROR_VALUE(err_str) (std::string(err_str).empty() ? TERNARY_CAST(ERROR_VALUE_SUCCESS) : TERNARY_CAST(ERROR_VALUE_FAILURE(err_str)))

namespace joystream {
namespace node {

Nan::Persistent<v8::Function> Plugin::constructor;

NAN_MODULE_INIT(Plugin::Init) {

  PluginAlertEncoder::InitAlertTypes(target);

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Plugin").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "start", Start);
  Nan::SetPrototypeMethod(tpl, "stop", Stop);
  Nan::SetPrototypeMethod(tpl, "pause", Pause);
  Nan::SetPrototypeMethod(tpl, "update_buyer_terms", UpdateBuyerTerms);
  Nan::SetPrototypeMethod(tpl, "update_seller_terms", UpdateSellerTerms);
  Nan::SetPrototypeMethod(tpl, "to_observe_mode", ToObserveMode);
  Nan::SetPrototypeMethod(tpl, "to_sell_mode", ToSellMode);
  Nan::SetPrototypeMethod(tpl, "to_buy_mode", ToBuyMode);
  Nan::SetPrototypeMethod(tpl, "post_torrent_plugin_status_updates", PostTorrentPluginStatusUpdates);
  Nan::SetPrototypeMethod(tpl, "post_peer_plugin_status_updates", PostPeerPluginStatusUpdates);
  Nan::SetPrototypeMethod(tpl, "stop_all_torrent_plugins", StopAllTorrentPlugins);
  Nan::SetPrototypeMethod(tpl, "pause_libtorrent", PauseLibtorrent);
  Nan::SetPrototypeMethod(tpl, "add_torrent", AddTorrent);
  Nan::SetPrototypeMethod(tpl, "remove_torrent", RemoveTorrent);
  Nan::SetPrototypeMethod(tpl, "pause_torrent", PauseTorrent);
  Nan::SetPrototypeMethod(tpl, "resume_torrent", ResumeTorrent);
  Nan::SetPrototypeMethod(tpl, "start_downloading", StartDownloading);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Plugin").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

libtorrent::node::AlertDecoder Plugin::getDecoder() const noexcept {
  return PluginAlertEncoder::alertEncoder;
}

boost::shared_ptr<libtorrent::plugin> Plugin::getPlugin() const noexcept {
  return boost::static_pointer_cast<libtorrent::plugin>(_plugin);
}

Plugin::Plugin(const boost::shared_ptr<extension::Plugin> & plugin)
  : _plugin(plugin) {
}

NAN_METHOD(Plugin::New) {

  NEW_OPERATOR_GUARD(info, constructor)
  ARGUMENTS_REQUIRE_NUMBER(0, minimumMessageId)

  // Create plugin
  Plugin * p = new Plugin(boost::make_shared<extension::Plugin>(minimumMessageId));

  // Wrap p in this
  p->Wrap(info.This());

  // Return this
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(Plugin::Start) {

  /**
  /// Get plugin reference

  Plugin * plugin = Nan::ObjectWrap::Unwrap<Plugin>(info.This());

  // read up on unwrapping, this vs holder, and also whether we have to tets resutl

  if(!plugin)
    //throw??

  if(info.Length < 2)
    Nan::ThrowError("......");

  // when decoding, do try catching and throw nan excption if it doent work

  // libtorrent::sha1_hash infoHash = infoHash::fromValue(info[0]);
  // std::shared_ptr<Nan::Callback> callback = std::make_shared<Nan::Callback>(info[1].As<v8::Function>());
  // _plugin->submit(extension::request::Start(infoHash, callback));

  plugin->submit(extension::request::RemoveTorrent(info_hash, [callback](const std::exception_ptr &){

    v8::Local<v8::Value> argv[] = {
        Nan::Null()
    };

    callback->Call(1, argv);
  }));
  */

}

NAN_METHOD(Plugin::Stop) {

  // _plugin->submit(extension::request::Stop(_infoHash, handler));

}

NAN_METHOD(Plugin::Pause) {

/**
  // Pause libtorrent session
  _plugin->submit(extension::request::PauseLibtorrent([this, paused]() {

      std::clog << "Libtorrent session paused" << std::endl;

      // Stop all plugins
      _plugin->submit(extension::request::StopAllTorrentPlugins([this, paused]() {

          std::clog << "All plugins stopped" << std::endl;

          // Service user callback
          paused();
      }));

  }));
  */

}

NAN_METHOD(Plugin::UpdateBuyerTerms) {
  //_plugin->submit(extension::request::UpdateBuyerTerms(_infoHash, terms, handler));
}

NAN_METHOD(Plugin::UpdateSellerTerms) {
  //_plugin->submit(extension::request::UpdateSellerTerms(_infoHash, terms, handler));
}

NAN_METHOD(Plugin::ToObserveMode) {
  //_plugin->submit(extension::request::ToObserveMode(_infoHash, handler));
}

NAN_METHOD(Plugin::ToSellMode) {

  /**
  libtorrent::sha1_hash infoHash;
  protocol_wire::SellerTerms terms;

  _plugin->submit(extension::request::ToSellMode(infoHash, terms));
  */

}

NAN_METHOD(Plugin::ToBuyMode) {

  /**
  libtorrent::sha1_hash infoHash;
  protocol_wire::BuyerTerms terms;

  _plugin->submit(extension::request::ToBuyMode(_infoHash, terms, handler));
  */

}

NAN_METHOD(Plugin::PostTorrentPluginStatusUpdates) {
  //_plugin->submit(extension::request::PostTorrentPluginStatusUpdates());
}

NAN_METHOD(Plugin::PostPeerPluginStatusUpdates) {

}

NAN_METHOD(Plugin::StopAllTorrentPlugins) {

  /**
  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  std::shared_ptr<Nan::Callback> callback;

  if(info.Length() > 0) {
      callback.reset(new Nan::Callback(info[0].As<v8::Function>()));
  }


    // Stop all plugins
    session_wrap->session_.plugin_->submit(joystream::extension::request::StopAllTorrentPlugins([callback]() {

        std::clog << "All plugins stopped" << std::endl;

        // Service user callback
        if(callback && !callback->IsEmpty()) {
            callback->Call(0, {});
        }
    }));

  info.GetReturnValue().Set(Nan::Undefined());
  */

}

NAN_METHOD(Plugin::PauseLibtorrent) {

  /**
  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());

  std::shared_ptr<Nan::Callback> callback;

  if(info.Length() > 0) {
      callback.reset(new Nan::Callback(info[0].As<v8::Function>()));
  }

  // Pause libtorrent session
  session_wrap->session_.plugin_->submit(joystream::extension::request::PauseLibtorrent([session_wrap, callback]() {

    std::clog << "Libtorrent session paused" << std::endl;

        if(callback && !callback->IsEmpty()) {
            callback->Call(0, {});
        }
  }));

  info.GetReturnValue().Set(Nan::Undefined());
  */

}

NAN_METHOD(Plugin::AddTorrent) {

  /*
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
NAN_METHOD(Plugin::RemoveTorrent) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)
  ARGUMENTS_REQUIRE_CALLBACK(1, managedCallback)

  // Create request
  joystream::extension::request::RemoveTorrent request(infoHash,
                                                       detail::CreateGenericSubroutineHandler(managedCallback));

  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::PauseTorrent) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)
  ARGUMENTS_REQUIRE_BOOLEAN(1, graceful)
  ARGUMENTS_REQUIRE_CALLBACK(2, managedCallback)

  // Create request
  joystream::extension::request::PauseTorrent request(infoHash,
                                                      graceful,
                                                      detail::CreateGenericSubroutineHandler(managedCallback));
  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::ResumeTorrent) {

}

NAN_METHOD(Plugin::StartDownloading) {

}

namespace detail {

  template<class ...Args>
  using ValueGenerator = v8::Local<v8::Value>(*)(const std::exception_ptr & ex, Args... args);

  namespace generic_value_generators {

    template<class ...Args>
    v8::Local<v8::Value> errorValueGn(const std::exception_ptr & ex, Args... args) {

      if(ex) { // failure

        try {
          std::rethrow_exception(ex);
        } catch(const std::exception & e) {
            return ERROR_VALUE_FAILURE(e.what());
        }

      } else // success
        return ERROR_VALUE_SUCCESS;
    }

    template<class ...Args>
    v8::Local<v8::Value> resultValueGn(const std::exception_ptr & ex, Args... args) {
      return RESULT_VALUE(ex);
    }

  }

  template<class ...Args>
  joystream::extension::request::SubroutineHandler CreateSubroutineHandler(const std::shared_ptr<Nan::Callback> & callback,
                                                                           const ValueGenerator<Args...> & errorValueGenerator,
                                                                           const ValueGenerator<Args...> & resultValueGenerator,
                                                                           Args... args) {

    auto bounded_error_value_generator = std::bind(errorValueGenerator, std::placeholders::_1, args...);
    auto bounded_result_value_generator = std::bind(resultValueGenerator, std::placeholders::_1, args...);

    return [callback, bounded_error_value_generator, bounded_result_value_generator] (const std::exception_ptr & ex) -> void {
      v8::Local<v8::Value> argv[] = { bounded_error_value_generator(ex), bounded_result_value_generator(ex) };
      callback->Call(2, argv);
    };
  }

  template<class ...Args>
  joystream::extension::request::SubroutineHandler CreateGenericSubroutineHandler(const std::shared_ptr<Nan::Callback> & callback) {

      return CreateSubroutineHandler<Args...>(callback,
                                              generic_value_generators::errorValueGn<Args...>,
                                              generic_value_generators::resultValueGn<Args...>);
  }

  /**
  /// This is what I would prefer, but does not build
  auto CreateGenericSubroutineHandler = std::bind(CreateSubroutineHandler<>,
                                                  std::placeholders::_1,
                                                  generic_value_generators::errorValueGn<>,
                                                  generic_value_generators::resultValueGn<>);
  */
}
}
}
