#ifndef ALERT_H
#define ALERT_H

#include <nan.h>
#include "torrent_handle.h"
#include <extension/Alert.hpp>

using namespace v8;

class AlertWrap: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(const libtorrent::alert* a);
      static const libtorrent::alert* Unwrap(const Local<Object>& obj) {
        return Nan::ObjectWrap::Unwrap<AlertWrap>(obj)->alert_;
      };

    private:
      explicit AlertWrap();
      ~AlertWrap();

      static NAN_METHOD(NewInstance);
      static NAN_METHOD(what);
      static NAN_METHOD(type);
      static NAN_METHOD(message);
      static NAN_METHOD(category);
      static NAN_METHOD(handle);

      static Nan::Persistent<Function> constructor;
      const libtorrent::alert* alert_;

};

#endif
