#include "add_torrent_params.h"

using namespace v8;

Nan::Persistent<Function> AddTorrentParamsWrap::constructor;

NAN_MODULE_INIT(AddTorrentParamsWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("AddTorrentParamsWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "resumeData", resume_data);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("AddTorrentParamsWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> AddTorrentParamsWrap::New(const libtorrent::add_torrent_params* atp) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    if(atp) {
        Nan::ObjectWrap::Unwrap<AddTorrentParamsWrap>(obj.ToLocalChecked())->add_torrent_params_ = atp;
    } else {
      obj = Nan::New<Object>();
    }
    return scope.Escape(obj.ToLocalChecked());
};

AddTorrentParamsWrap::AddTorrentParamsWrap() {
  add_torrent_params_ = NULL;
};

AddTorrentParamsWrap::~AddTorrentParamsWrap() {
  if (add_torrent_params_ != NULL)
    delete add_torrent_params_;
};

NAN_METHOD(AddTorrentParamsWrap::NewInstance) {
  Nan::HandleScope scope;

  AddTorrentParamsWrap* obj = new AddTorrentParamsWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};

NAN_METHOD(AddTorrentParamsWrap::resume_data) {
    Nan::HandleScope scope;

    const libtorrent::add_torrent_params* atp = AddTorrentParamsWrap::Unwrap(info.This());

    // Not sure about this, need review...
    Nan::MaybeLocal<String> rd = Nan::New<String>(atp->resume_data.data());
    info.GetReturnValue().Set(rd.ToLocalChecked());
};
