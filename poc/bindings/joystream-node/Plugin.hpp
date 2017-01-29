/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#ifndef JOYSTREAM_NODE_PLUGIN_HPP
#define JOYSTREAM_NODE_PLUGIN_HPP

#include "libtorrent-node/plugin.hpp"

#include <boost/shared_ptr.hpp>

namespace joystream {
namespace extension {
  class Plugin;
}
namespace node {

/**
 * @brief Binding for joystream extension.
 */
class Plugin : public libtorrent::node::plugin {

public:

  static NAN_MODULE_INIT(Init);

  virtual libtorrent::node::AlertDecoder getDecoder() const noexcept;

  virtual boost::shared_ptr<libtorrent::plugin> getPlugin() const noexcept;

private:

  boost::shared_ptr<extension::Plugin> _plugin;

  Plugin(const boost::shared_ptr<extension::Plugin> & plugin);

  static Nan::Persistent<v8::Function> constructor;

  static NAN_METHOD(New);
  static NAN_METHOD(Start);
  static NAN_METHOD(Stop);
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

#endif // JOYSTREAM_NODE_PLUGIN_HPP
