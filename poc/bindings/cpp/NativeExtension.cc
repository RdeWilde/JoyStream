#include "session.h"
#include "torrent_handle.h"
#include "torrent_status.h"
#include <nan.h>

// NativeExtension.cc represents the top level of the module.
// C++ constructs that are exposed to javascript are exported here

NAN_MODULE_INIT(InitAll) {
  TorrentStatusWrap::Init(target);
  TorrentHandleWrap::Init(target);
  SessionWrap::Init(target);
}

NODE_MODULE(NativeExtension, InitAll)
