#ifndef ADD_TORRENT_PARAMS_H
#define ADD_TORRENT_PARAMS_H

#include <nan.h>
#include <libtorrent/add_torrent_params.hpp>

using namespace v8;

class AddTorrentParamsWrap: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(const libtorrent::add_torrent_params& atp);
      static libtorrent::add_torrent_params* Unwrap(const Local<Object>& obj) {
        AddTorrentParamsWrap* atp = Nan::ObjectWrap::Unwrap<AddTorrentParamsWrap>(obj);
        return &atp->add_torrent_params_;
      };

    private:
      libtorrent::add_torrent_params add_torrent_params_;
      static Nan::Persistent<Function> constructor;

      static NAN_METHOD(NewInstance);
      static NAN_METHOD(resume_data);

};

#endif
