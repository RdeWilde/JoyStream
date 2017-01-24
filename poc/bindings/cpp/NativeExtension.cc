#include "session.h"
#include "alert.h"
#include "torrent_handle.h"
#include "torrent_info.h"
#include "bencode.h"
#include "SellerTerms.hpp"
#include "BuyerTerms.hpp"
#include "payment_channel.hpp"
#include <nan.h>

// NativeExtension.cc represents the top level of the module.
// C++ constructs that are exposed to javascript are exported here

NAN_MODULE_INIT(InitExtension);

NAN_MODULE_INIT(InitAll) {

  SET_VAL(target, "BEncode", Nan::GetFunction(Nan::New<v8::FunctionTemplate>(BEncode)).ToLocalChecked());  
  AlertWrap::Init(target);
  TorrentHandle::Init(target);
  TorrentInfo::Init(target);
  libtorrent::node::SessionWrap::Init(target);

  joystream::node::payment_channel::Init(target);

  InitExtension(target);
}

NODE_MODULE(NativeExtension, InitAll)
