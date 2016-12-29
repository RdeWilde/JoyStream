#ifndef ADD_TORRENT_PARAMS_H
#define ADD_TORRENT_PARAMS_H

#include <nan.h>
#include <libtorrent/add_torrent_params.hpp>

using namespace v8;

class AddTorrentParamsWrap: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(libtorrent::add_torrent_params atp);
      static libtorrent::add_torrent_params* Unwrap(const Local<Object>& obj) {
        AddTorrentParamsWrap* atp = Nan::ObjectWrap::Unwrap<AddTorrentParamsWrap>(obj);
        return &atp->add_torrent_params_;
      };

    private:
      explicit AddTorrentParamsWrap();
      ~AddTorrentParamsWrap();

      static NAN_METHOD(NewInstance);
      static NAN_METHOD(resume_data);

      static Nan::Persistent<Function> constructor;
      libtorrent::add_torrent_params add_torrent_params_;

};

#endif
