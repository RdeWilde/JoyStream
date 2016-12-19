#include "session.h"
#include "torrent_handle.h"
#include <nan.h>

// NativeExtension.cc represents the top level of the module.
// C++ constructs that are exposed to javascript are exported here

NAN_MODULE_INIT(InitAll) {
  TorrentHandleWrap::Init(target);
  SessionWrap::Init(target);
}

NODE_MODULE(NativeExtension, InitAll)
