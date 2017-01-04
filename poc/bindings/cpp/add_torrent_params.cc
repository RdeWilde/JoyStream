#include "add_torrent_params.h"

using namespace v8;

Nan::Persistent<Function> AddTorrentParamsWrap::constructor;

NAN_MODULE_INIT(AddTorrentParamsWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("AddTorrentParams").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();
  Nan::SetAccessor(inst, Nan::New("resumeData").ToLocalChecked(), AddTorrentParamsWrap::resume_data);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("AddTorrentParams").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> AddTorrentParamsWrap::New(const libtorrent::add_torrent_params& atp) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    Nan::ObjectWrap::Unwrap<AddTorrentParamsWrap>(obj.ToLocalChecked())->add_torrent_params_ = atp;

    return scope.Escape(obj.ToLocalChecked());
};

NAN_METHOD(AddTorrentParamsWrap::NewInstance) {

  AddTorrentParamsWrap* obj = new AddTorrentParamsWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};

NAN_GETTER(AddTorrentParamsWrap::resume_data) {

    libtorrent::add_torrent_params* atp = AddTorrentParamsWrap::Unwrap(info.This());

    // Not sure about this, need review...
    Nan::MaybeLocal<String> rd = Nan::New<String>(atp->resume_data.data(), atp->resume_data.size());
    info.GetReturnValue().Set(rd.ToLocalChecked());
};
