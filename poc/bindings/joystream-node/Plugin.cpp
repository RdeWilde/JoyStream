/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include "Plugin.hpp"
#include "PluginAlertEncoder.hpp"
#include "BuyerTerms.hpp"
#include "SellerTerms.hpp"
#include "PrivateKey.hpp"
#include "PubKeyHash.hpp"
#include "libtorrent-node/utils.hpp"
#include "libtorrent-node/sha1_hash.hpp"
#include "libtorrent-node/add_torrent_params.hpp"
#include "libtorrent-node/endpoint.hpp"

#include <extension/extension.hpp>

namespace detail {
namespace subroutine_handler {
  joystream::extension::request::SubroutineHandler CreateGenericHandler(const std::shared_ptr<Nan::Callback> & callback);
}
namespace no_exception_subroutine_handler {
  joystream::extension::request::NoExceptionSubroutineHandler CreateGenericHandler(const std::shared_ptr<Nan::Callback> & callback);
}

  joystream::extension::request::AddTorrent::AddTorrentHandler CreateAddTorrentHandler(const std::shared_ptr<Nan::Callback> & callback);

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
  Nan::SetPrototypeMethod(tpl, "update_buyer_terms", UpdateBuyerTerms);
  Nan::SetPrototypeMethod(tpl, "update_seller_terms", UpdateSellerTerms);
  Nan::SetPrototypeMethod(tpl, "to_observe_mode", ToObserveMode);
  Nan::SetPrototypeMethod(tpl, "to_sell_mode", ToSellMode);
  Nan::SetPrototypeMethod(tpl, "to_buy_mode", ToBuyMode);
  Nan::SetPrototypeMethod(tpl, "post_torrent_plugin_status_updates", PostTorrentPluginStatusUpdates);
  Nan::SetPrototypeMethod(tpl, "post_peer_plugin_status_updates", PostPeerPluginStatusUpdates);
  Nan::SetPrototypeMethod(tpl, "pause_libtorrent", PauseLibtorrent);
  Nan::SetPrototypeMethod(tpl, "add_torrent", AddTorrent);
  Nan::SetPrototypeMethod(tpl, "remove_torrent", RemoveTorrent);
  Nan::SetPrototypeMethod(tpl, "pause_torrent", PauseTorrent);
  Nan::SetPrototypeMethod(tpl, "resume_torrent", ResumeTorrent);
  Nan::SetPrototypeMethod(tpl, "start_downloading", StartDownloading);
  Nan::SetPrototypeMethod(tpl, "start_uploading", StartUploading);

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

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)
  ARGUMENTS_REQUIRE_CALLBACK(1, managedCallback)

  // Create request
  joystream::extension::request::Start request(infoHash,
                                               detail::subroutine_handler::CreateGenericHandler(managedCallback));

  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::Stop) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)
  ARGUMENTS_REQUIRE_CALLBACK(1, managedCallback)

  // Create request
  joystream::extension::request::Stop request(infoHash,
                                              detail::subroutine_handler::CreateGenericHandler(managedCallback));

  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::UpdateBuyerTerms) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)
  ARGUMENTS_REQUIRE_DECODED(1, buyerTerms, protocol_wire::BuyerTerms, node::buyer_terms::decode)
  ARGUMENTS_REQUIRE_CALLBACK(2, managedCallback)

  // Create request
  joystream::extension::request::UpdateBuyerTerms request(infoHash,
                                                          buyerTerms,
                                                          detail::subroutine_handler::CreateGenericHandler(managedCallback));

  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::UpdateSellerTerms) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)
  ARGUMENTS_REQUIRE_DECODED(1, sellerTerms, protocol_wire::SellerTerms, node::seller_terms::decode)
  ARGUMENTS_REQUIRE_CALLBACK(2, managedCallback)

  // Create request
  joystream::extension::request::UpdateSellerTerms request(infoHash,
                                                           sellerTerms,
                                                           detail::subroutine_handler::CreateGenericHandler(managedCallback));

  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::ToObserveMode) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)
  ARGUMENTS_REQUIRE_CALLBACK(1, managedCallback)

  // Create request
  joystream::extension::request::ToObserveMode request(infoHash,
                                                       detail::subroutine_handler::CreateGenericHandler(managedCallback));

  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::ToSellMode) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)
  ARGUMENTS_REQUIRE_DECODED(1, sellerTerms, protocol_wire::SellerTerms, node::seller_terms::decode)
  ARGUMENTS_REQUIRE_CALLBACK(2, managedCallback)

  // Create request
  joystream::extension::request::ToSellMode request(infoHash,
                                                    sellerTerms,
                                                    detail::subroutine_handler::CreateGenericHandler(managedCallback));

  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::ToBuyMode) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)
  ARGUMENTS_REQUIRE_DECODED(1, buyerTerms, protocol_wire::BuyerTerms, node::buyer_terms::decode)
  ARGUMENTS_REQUIRE_CALLBACK(2, managedCallback)

  // Create request
  joystream::extension::request::ToBuyMode request(infoHash,
                                                   buyerTerms,
                                                   detail::subroutine_handler::CreateGenericHandler(managedCallback));

  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::PostTorrentPluginStatusUpdates) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)

  // Create request
  joystream::extension::request::PostTorrentPluginStatusUpdates request;

  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::PostPeerPluginStatusUpdates) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)

  // Create request
  joystream::extension::request::PostPeerPluginStatusUpdates request(infoHash);

  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::PauseLibtorrent) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_CALLBACK(0, managedCallback)

  // Create request
  joystream::extension::request::PauseLibtorrent request(detail::no_exception_subroutine_handler::CreateGenericHandler(managedCallback));

  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::AddTorrent) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  libtorrent::add_torrent_params addTorrentParams;
  // this requires => ARGUMENTS_REQUIRE_DECODED(0, addTorrentParams, libtorrent::add_torrent_params, libtorrent::node::add_torrent_params::decode)
  ARGUMENTS_REQUIRE_CALLBACK(1, managedCallback)

  // Create request
  joystream::extension::request::AddTorrent request(addTorrentParams,
                                                    detail::CreateAddTorrentHandler(managedCallback));

  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::RemoveTorrent) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)
  ARGUMENTS_REQUIRE_CALLBACK(1, managedCallback)

  // Create request
  joystream::extension::request::RemoveTorrent request(infoHash,
                                                       detail::subroutine_handler::CreateGenericHandler(managedCallback));

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
                                                      detail::subroutine_handler::CreateGenericHandler(managedCallback));
  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::ResumeTorrent) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)
  ARGUMENTS_REQUIRE_CALLBACK(1, managedCallback)

  // Create request
  joystream::extension::request::ResumeTorrent request(infoHash,
                                                       detail::subroutine_handler::CreateGenericHandler(managedCallback));
  // Submit request
  plugin->_plugin->submit(request);

  RETURN_VOID
}

NAN_METHOD(Plugin::StartDownloading) {

}

NAN_METHOD(Plugin::StartUploading) {

  // Get validated parameters
  GET_THIS_PLUGIN(plugin)
  ARGUMENTS_REQUIRE_DECODED(0, infoHash, libtorrent::sha1_hash, libtorrent::node::sha1_hash::decode)

  libtorrent::tcp::endpoint endPoint;
  //ARGUMENTS_REQUIRE_DECODED(1, endPoint, libtorrent::tcp::endpoint, libtorrent::node::endpoint::fromObject)
  ARGUMENTS_REQUIRE_DECODED(2, buyerTerms, protocol_wire::BuyerTerms, joystream::node::buyer_terms::decode)
  ARGUMENTS_REQUIRE_DECODED(3, contractSk, Coin::PrivateKey, joystream::node::private_key::decode)
  ARGUMENTS_REQUIRE_DECODED(4, finalPkHash, Coin::PubKeyHash, joystream::node::pubkey_hash::decode)
  ARGUMENTS_REQUIRE_CALLBACK(5, managedCallback)

  Coin::KeyPair contractKeyPair(contractSk);

  // Create request
  joystream::extension::request::StartUploading request(infoHash,
                                                        endPoint,
                                                        buyerTerms,
                                                        contractKeyPair,
                                                        finalPkHash,
                                                        detail::subroutine_handler::CreateGenericHandler(managedCallback));

    // Submit request
    plugin->_plugin->submit(request);

    RETURN_VOID
}

namespace detail {

  /// SubroutineHandler
  namespace subroutine_handler {

    template<class ...Args>
    using ValueGenerator = v8::Local<v8::Value>(*)(const std::exception_ptr & ex, Args... args);

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

    v8::Local<v8::Value> errorValueGn(const std::exception_ptr & ex) {

      if(ex) { // failure

        try {
          std::rethrow_exception(ex);
        } catch(const std::exception & e) {
            return ERROR_VALUE_FAILURE(e.what());
        }

      } else // success
        return ERROR_VALUE_SUCCESS;
    }

    v8::Local<v8::Value> resultValueGn(const std::exception_ptr & ex) {
      return RESULT_VALUE(ex);
    }

    joystream::extension::request::SubroutineHandler CreateGenericHandler(const std::shared_ptr<Nan::Callback> & callback) {
        return CreateSubroutineHandler(callback, &errorValueGn, &resultValueGn);
    }
  }

  /// NoExceptionSubroutineHandler

  namespace no_exception_subroutine_handler {

    template<class ...Args>
    using ValueGenerator = v8::Local<v8::Value>(*)(Args... args);

    template<class ...Args>
    joystream::extension::request::NoExceptionSubroutineHandler CreateHandler(const std::shared_ptr<Nan::Callback> & callback,
                                                                              const ValueGenerator<Args...> errorValueGenerator,
                                                                              const ValueGenerator<Args...> resultValueGenerator,
                                                                              Args... args) {

      auto bounded_error_value_generator = std::bind(errorValueGenerator, args...);
      auto bounded_result_value_generator = std::bind(resultValueGenerator, args...);

      return [callback, bounded_error_value_generator, bounded_result_value_generator] () -> void {
        v8::Local<v8::Value> argv[] = { bounded_error_value_generator(), bounded_result_value_generator() };
        callback->Call(2, argv);
      };
    }

    v8::Local<v8::Value> genericErrorValueGenerator() {
      return ERROR_VALUE_SUCCESS;
    }

    v8::Local<v8::Value> genericResultValueGenerator() {
      return RESULT_VALUE_SUCCESS;
    }

    joystream::extension::request::NoExceptionSubroutineHandler CreateGenericHandler(const std::shared_ptr<Nan::Callback> & callback) {
      return CreateHandler<>(callback, &genericErrorValueGenerator, &genericResultValueGenerator);
    }

  }

  /// Custome handlers

  joystream::extension::request::AddTorrent::AddTorrentHandler CreateAddTorrentHandler(const std::shared_ptr<Nan::Callback> & callback) {

    return [callback] (libtorrent::error_code & ec, libtorrent::torrent_handle & h) -> void {

      // Use error_code::encode(ec) when its ready
      std::string err = (ec ? "<convert error_code to string>" : "");

      v8::Local<v8::Value> argv[] = { ERROR_VALUE(err), RESULT_VALUE(ec) };
      callback->Call(2, argv);
    };

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
