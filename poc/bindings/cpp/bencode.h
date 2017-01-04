#ifndef BENCODE_H
#define BENCODE_H

#include <nan.h>
#include <libtorrent/bencode.hpp>
#include <libtorrent/bdecode.hpp>
#include <libtorrent/entry.hpp>

int BufferToBDecodeNode(v8::Local<v8::Value>, libtorrent::bdecode_node&);
int ValueToEntry(v8::Local<v8::Value>, libtorrent::entry&);
v8::Local<v8::Value> EntryToValue(libtorrent::entry&);

NAN_METHOD(BEncode);
NAN_METHOD(BDecode);

#endif
