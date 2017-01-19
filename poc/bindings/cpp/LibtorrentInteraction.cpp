/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 19 2017
 */

#include "LibtorrentInteraction.hpp"
#include "utils.hpp"

namespace joystream {
namespace node {
namespace LibtorrentInteraction {

  #define SET_LIBTORRENT_INTERACTION(o, name) SET_VAL(o, #name, createValue(name));

  NAN_MODULE_INIT(Init) {

    SET_LIBTORRENT_INTERACTION(target, extension::TorrentPlugin::LibtorrentInteraction::None);
    SET_LIBTORRENT_INTERACTION(target, extension::TorrentPlugin::LibtorrentInteraction::BlockUploading);
    SET_LIBTORRENT_INTERACTION(target, extension::TorrentPlugin::LibtorrentInteraction::BlockDownloading);
    SET_LIBTORRENT_INTERACTION(target, extension::TorrentPlugin::LibtorrentInteraction::BlockUploadingAndDownloading);

  }

  v8::Local<v8::Value> createValue(extension::TorrentPlugin::LibtorrentInteraction interaction) {

    switch(interaction) {
      case extension::TorrentPlugin::LibtorrentInteraction::None: return Nan::New(0);
      case extension::TorrentPlugin::LibtorrentInteraction::BlockUploading: return Nan::New(1);
      case extension::TorrentPlugin::LibtorrentInteraction::BlockDownloading: return Nan::New(2);
      case extension::TorrentPlugin::LibtorrentInteraction::BlockUploadingAndDownloading: return Nan::New(3);
      default:
        assert(false);
    }

  }

  extension::TorrentPlugin::LibtorrentInteraction fromValue(const v8::Local<v8::Value> & v) {

    uint32_t value = To<uint32_t>(v);

    switch(value) {
      case 0: return extension::TorrentPlugin::LibtorrentInteraction::None;
      case 1: return extension::TorrentPlugin::LibtorrentInteraction::BlockUploading;
      case 2: return extension::TorrentPlugin::LibtorrentInteraction::BlockDownloading;
      case 3: return extension::TorrentPlugin::LibtorrentInteraction::BlockUploadingAndDownloading;
      default:
        assert(false);
    }

  }

}
}
}
