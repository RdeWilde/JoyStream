#include <addon/extension/PeerPluginStatus.hpp>

using namespace v8;

namespace joystream {
  namespace addon {
    namespace extension {

      Nan::Persistent<Function> PeerPluginStatus::constructor;

      NAN_MODULE_INIT(PeerPluginStatus::Init) {
        Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
        tpl->SetClassName(Nan::New("PeerPluginStatus").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        /*Local<ObjectTemplate> inst = tpl->InstanceTemplate();
        Nan::SetAccessor(inst, Nan::New("infoHash").ToLocalChecked(), PeerPluginStatus::info_hash);*/

        constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
        Nan::Set(target, Nan::New("PeerPluginStatus").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
      };

      Local<Object> PeerPluginStatus::New(const joystream::extension::status::PeerPlugin& pp) {
          Nan::EscapableHandleScope scope;

          Local<Function> cons = Nan::New(constructor);
          Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

          Nan::ObjectWrap::Unwrap<PeerPluginStatus>(obj.ToLocalChecked())->peer_plugin_status_ = pp;

          return scope.Escape(obj.ToLocalChecked());
      };

      NAN_METHOD(PeerPluginStatus::NewInstance) {
        PeerPluginStatus* obj = new PeerPluginStatus();
        obj->Wrap(info.This());

        info.GetReturnValue().Set(info.This());
      };
    }
  }
}
