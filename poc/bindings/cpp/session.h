#ifndef NATIVE_EXTENSION_GRAB_H
#define NATIVE_EXTENSION_GRAB_H

#include <nan.h>
#include <iostream>
#include <joystream_libtorrent_session/Session.hpp>


// Example with node ObjectWrap
// Based on https://nodejs.org/api/addons.html#addons_wrapping_c_objects but using NAN
class SessionWrap : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);

  private:
    explicit SessionWrap();
    ~SessionWrap();

    static NAN_METHOD(New);
    static NAN_METHOD(AddTorrent);
    static NAN_METHOD(RemoveTorrent);
    static NAN_METHOD(ListenPort);
    static NAN_METHOD(PostTorrentUpdates);
    static NAN_METHOD(Pause);
    static NAN_METHOD(IsPaused);
    static NAN_METHOD(Resume);
    static NAN_METHOD(FindTorrent);
    static NAN_METHOD(PopAlerts);
    static NAN_METHOD(SetAlertNotify);
    static Nan::Persistent<v8::Function> constructor;
    Session session_;

    class AddTorrentWorker : public Nan::AsyncWorker {
      public:
        AddTorrentWorker(Nan::Callback *callback, libtorrent::session* s, libtorrent::add_torrent_params params)
          : AsyncWorker(callback), session_(s), params_(params) {}
        ~AddTorrentWorker() {}

      // Executed inside the worker-thread.
      // It is not safe to access V8, or V8 data structures
      // here, so everything we need for input and output
      // should go on `this`.
      void Execute () {
        th_ = session_->add_torrent(params_, ec_);
      }

      // Executed when the async work is complete
      // this function will be run inside the main event loop
      // so it is safe to use V8 again
      void HandleOKCallback () {
        Nan::HandleScope scope;

        printf("Torrent added created \n");

        v8::Local<v8::Value> argv[] = {
            Nan::Null()
          , Nan::New<v8::Number>(1)
        };

        callback->Call(2, argv);
      }

     private:
       libtorrent::session* session_;
       libtorrent::add_torrent_params params_;
       libtorrent::torrent_handle th_;
       libtorrent::error_code ec_;
    };
};

#endif
