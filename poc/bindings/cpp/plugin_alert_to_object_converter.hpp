/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 14 2017
 */

#include <nan.h>

namespace joystream {
namespace node_addon {

/**
 * @brief Plugin alert to javascript object converter.
 *
 * A plugin addon must offer a wrapped instance of 'PluginAlertToObjectConverter',
 * through 'PluginAlertToObjectConverter::NewInstance', for use with 'session::pop_alerts'.
 */

class plugin_alert_to_object_converter : public Nan::ObjectWrap {

public:

  typedef void(*converter)(const libtorrent::alert &, v8::Local<v8::Object> & o, bool success);

  static v8::Local<v8::Value> NewInstance(converter func);

private:

  converter _func;
};

}
}
