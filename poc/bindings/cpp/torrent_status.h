#ifndef TORRENT_STATUS_H
#define TORRENT_STATUS_H

#include <nan.h>
#include <libtorrent/torrent_status.hpp>

v8::Local<v8::Value> torrent_status_to_object(const libtorrent::torrent_status& st);

#endif
