#include <addon/common/OutPoint.hpp>

namespace joystream {
namespace addon {
namespace common {

Nan::Persistent<v8::Function> OutPoint::constructor;

NAN_MODULE_INIT(OutPoint::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("OutPoint").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("OutPoint").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

v8::Local<v8::Object> OutPoint::NewInstance(const Coin::typesafeOutPoint &outpoint) {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto instance = cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked();
    OutPoint* op = ObjectWrap::Unwrap<OutPoint>(instance);
    op->_outpoint = outpoint;
    return scope.Escape(instance);
}

bool OutPoint::IsInstance(v8::Object &obj) {
    return obj.GetPrototype() == constructor;
}

Coin::typesafeOutPoint OutPoint::outpoint() const {
    return _outpoint;
}

NAN_METHOD(OutPoint::New) {
  if (info.IsConstructCall()) {
    OutPoint *obj = new OutPoint();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked());
  }
}

}}}
