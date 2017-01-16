/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 14 2017
 */

#include "plugin_alert_to_object_converter.hpp"

namespace joystream {
namespace node_addon {

v8::Local<v8::Value> plugin_alert_to_object_converter::NewInstance(converter func) {

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::ObjectTemplate>();
  //tpl->SetClassName(Nan::New("AddTorrentParams").ToLocalChecked());
  //tpl->InstanceTemplate()->SetInternalFieldCount(1);
}




}
}
