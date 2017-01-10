#include <addon/protocol_session/ConnectionTCPEndpoint.hpp>

namespace joystream {
  namespace addon {
    namespace protocol_session {

      Nan::Persistent<v8::Function> ConnectionTCPEndpoint::constructor;

      NAN_MODULE_INIT(ConnectionTCPEndpoint::Init) {
        v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(NewInstance);
        tpl->SetClassName(Nan::New("ConnectionTCPEndpoint").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);


        constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
        Nan::Set(target, Nan::New("ConnectionTCPEndpoint").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
      };

      v8::Local<v8::Object> ConnectionTCPEndpoint::New(const joystream::protocol_session::status::Connection<libtorrent::tcp::endpoint>& c) {
          Nan::EscapableHandleScope scope;

          v8::Local<v8::Function> cons = Nan::New(constructor);
          Nan::MaybeLocal<v8::Object> obj = cons->NewInstance(Nan::GetCurrentContext());

          Nan::ObjectWrap::Unwrap<ConnectionTCPEndpoint>(obj.ToLocalChecked())->connection_ = c;

          return scope.Escape(obj.ToLocalChecked());
      };

      NAN_METHOD(ConnectionTCPEndpoint::NewInstance) {
        ConnectionTCPEndpoint* obj = new ConnectionTCPEndpoint();
        obj->Wrap(info.This());

        info.GetReturnValue().Set(info.This());
      };
    }
  }
}
