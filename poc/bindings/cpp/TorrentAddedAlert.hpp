/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include "TorrentAlert.hpp"
#include <libtorrent/alert_types.hpp>

namespace joystream {
namespace node_addon {

class TorrentAddedAlert : public TorrentAlert {

public:

  static NAN_MODULE_INIT(Init);

  static v8::Local<v8::Object> NewInstance(const libtorrent::torrent_added_alert * alert);

private:

  static Nan::Persistent<v8::Function> constructor;

  static NAN_METHOD(New);

  TorrentAddedAlert(const libtorrent::torrent_handle & handle);

  ALERT_WRAPPER_IMPL(libtorrent::torrent_added_alert)
  virtual std::string message() const override;
};

}
}
