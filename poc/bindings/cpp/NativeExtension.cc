#include "session.h"
#include "alert.h"
#include "torrent_handle.h"
#include "torrent_info.h"
#include "bencode.h"
#include "SellerTerms.hpp"
#include "BuyerTerms.hpp"
#include <nan.h>

// NativeExtension.cc represents the top level of the module.
// C++ constructs that are exposed to javascript are exported here

NAN_MODULE_INIT(InitExtension);

NAN_MODULE_INIT(InitAll) {

  Nan::Set(target, Nan::New<v8::String>("BEncode").ToLocalChecked(),
  Nan::GetFunction(Nan::New<v8::FunctionTemplate>(BEncode)).ToLocalChecked());
  PeerInfoWrap::Init(target);
  EndpointWrap::Init(target);
  AddTorrentParamsWrap::Init(target);
  AlertWrap::Init(target);
  TorrentHandle::Init(target);
  TorrentInfo::Init(target);
  libtorrent::node::SessionWrap::Init(target);

  InitExtension(target);
}

NODE_MODULE(NativeExtension, InitAll)
