#ifndef ALERT_H
#define ALERT_H

#include <nan.h>
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
      //static NAN_METHOD(handle);

      static Nan::Persistent<Function> constructor;
      const libtorrent::alert* alert_;

      /* some template trickery to workaround alert_cast stuff */
      /* getting the handle */
      /*template <class T> static const libtorrent::torrent_handle* getHandleInternal(const libtorrent::alert& p)
      {
        auto obj = dynamic_cast<const T*>(&p);
        return obj ? &obj->handle : nullptr;
      };

      template <class T1, class T2, class...Ts> static const libtorrent::torrent_handle* getHandleInternal(const libtorrent::alert& p)
      {
        auto v = getHandleInternal<T1>(p);
        return v ? v : getHandleInternal<T2, Ts...>(p);
      };

      template <class... Ts> static libtorrent::torrent_handle getHandle(const libtorrent::alert& p) // C++14 can return auto here
      {
        auto handle = getHandleInternal<Ts...>(p);

        if (!handle)
          throw std::exception();

          return *handle;
      };*/

      /* getting the object */
      template <class T> static const libtorrent::alert* getObjectInternal(const libtorrent::alert& p)
      {
        auto obj = libtorrent::alert_cast<const T>(&p);

        return obj;
      };

      template <class T1, class T2, class...Ts> static const libtorrent::alert* getObjectInternal(const libtorrent::alert& p)
      {
        auto v = getObjectInternal<T1>(p);
        return v ? v : getObjectInternal<T2, Ts...>(p);
      };

      template <class... Ts> static const libtorrent::alert* getObject(const libtorrent::alert& p) // C++14 can return auto here
      {
        auto obj = getObjectInternal<Ts...>(p);

        if (!obj)
          throw std::exception();

        return obj;
      };
};

#endif
