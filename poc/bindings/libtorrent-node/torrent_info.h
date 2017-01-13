#ifndef LIBTORRENT_NODE_TORRENT_INFO_HPP
#define LIBTORRENT_NODE_TORRENT_INFO_HPP

#include <nan.h>
#include <boost/optional.hpp>
#include <libtorrent/torrent_info.hpp>

<<<<<<< HEAD:poc/bindings/libtorrent-node/torrent_info.h
using namespace v8;

class TorrentInfo: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(boost::shared_ptr<const libtorrent::torrent_info> ti);
      static boost::shared_ptr<const libtorrent::torrent_info> Unwrap(const Local<Object>& obj) {
        TorrentInfo* ti = Nan::ObjectWrap::Unwrap<TorrentInfo>(obj);
        return ti->torrent_info_;
=======
class TorrentInfoWrap: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static v8::Local<v8::Object> New(boost::shared_ptr<const libtorrent::torrent_info>& ti);
      static const libtorrent::torrent_info* Unwrap(const v8::Local<v8::Object>& obj) {
        TorrentInfoWrap* ti = Nan::ObjectWrap::Unwrap<TorrentInfoWrap>(obj);
        return ti->torrent_info_.get();
>>>>>>> Adding name() binding:poc/bindings/cpp/torrent_info.h
      };

    private:
      boost::shared_ptr<const libtorrent::torrent_info> torrent_info_;
      static Nan::Persistent<v8::Function> constructor;

      static NAN_METHOD(NewInstance);
      static NAN_METHOD(name);
      static NAN_METHOD(total_size);

};

#endif // LIBTORRENT_NODE_TORRENT_INFO_HPP
