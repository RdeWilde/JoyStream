/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include <nan.h>

#include <extension/extension.hpp>

namespace joystream {
namespace node_addon {

/**
 * @brief Binding for joystream extension.
 */

// we need to inherit from someting else to meake interoperable with libtorrent-node bindigns
// so that it can be used with session::add_extension
class Plugin : public Nan::ObjectWrap {

public:

  static NAN_MODULE_INIT(Init);
  static v8::Local<v8::Object> NewIntance(const boost::shared_ptr<extension::Plugin> & plugin);

private:

  // We store a shared pointer, rather than weak, some what presumptiously.
  boost::shared_ptr<extension::Plugin> _plugin;

  static Nan::Persistent<v8::Function> constructor;

  static NAN_METHOD(New);

  static NAN_METHOD(AlertConverter);

  // Functions fro submitting different requests
  static NAN_METHOD(Start);
  static NAN_METHOD(Stop);
  static NAN_METHOD(Pause);
  static NAN_METHOD(UpdateBuyerTerms);
  static NAN_METHOD(UpdateSellerTerms);
  static NAN_METHOD(ToObserveMode);
  static NAN_METHOD(ToSellMode);
  static NAN_METHOD(ToBuyMode);
  static NAN_METHOD(PostTorrentPluginStatusUpdates);
  static NAN_METHOD(PostPeerPluginStatusUpdates);
  static NAN_METHOD(StopAllTorrentPlugins);
  static NAN_METHOD(PauseLibtorrent);
  static NAN_METHOD(AddTorrent);
  static NAN_METHOD(RemoveTorrent);
  static NAN_METHOD(PauseTorrent);
  static NAN_METHOD(ResumeTorrent);
  static NAN_METHOD(StartDownloading);
};

}
}
