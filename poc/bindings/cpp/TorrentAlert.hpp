/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include "Alert.hpp"
#include <libtorrent/torrent_handle.hpp>

namespace joystream {
namespace node_addon {

/**
* @brief Base class for all torrent_alert binding classes,
* same considerations as in 'Alert' apply.
*/
class TorrentAlert : public Alert {

  libtorrent::torrent_handle _handle;

protected:

  TorrentAlert(const libtorrent::torrent_handle & handle);

  static void Init(const std::string & className,
                   v8::Local<v8::FunctionTemplate> & t,
                   int internalFieldCount = 1);

  static NAN_METHOD(handle);

  virtual std::string message() const override;
};

}
}
