#include "alert.h"

using namespace v8;

Nan::Persistent<Function> AlertWrap::constructor;

NAN_MODULE_INIT(AlertWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("AlertWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "what", what);
  Nan::SetPrototypeMethod(tpl, "type", type);
  Nan::SetPrototypeMethod(tpl, "message", message);
  Nan::SetPrototypeMethod(tpl, "category", category);
  //Nan::SetPrototypeMethod(tpl, "handle", handle);


  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("AlertWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> AlertWrap::New(const libtorrent::alert* a) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    if(a) {
        try {
            auto casted = getObject <libtorrent::add_torrent_alert,
                            libtorrent::torrent_removed_alert,
                            libtorrent::save_resume_data_alert,
                            libtorrent::save_resume_data_failed_alert,
                            libtorrent::metadata_received_alert,
                            libtorrent::torrent_finished_alert,
                            joystream::extension::alert::RequestResult,
                            joystream::extension::alert::PluginStatus>(*a);
            Nan::ObjectWrap::Unwrap<AlertWrap>(obj.ToLocalChecked())->alert_ = casted;
        } catch(std::exception e) {
            /* casting failed */
            printf("Unknown alert...");
            Nan::ObjectWrap::Unwrap<AlertWrap>(obj.ToLocalChecked())->alert_ = a;
        }
    } else {
      obj = Nan::New<Object>();
    }
    return scope.Escape(obj.ToLocalChecked());
};

AlertWrap::AlertWrap() {
  alert_ = NULL;
};

AlertWrap::~AlertWrap() {
  if (alert_ != NULL)
    delete alert_;
};

NAN_METHOD(AlertWrap::NewInstance) {
  Nan::HandleScope scope;

  AlertWrap* obj = new AlertWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};

NAN_METHOD(AlertWrap::what) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<String>(AlertWrap::Unwrap(info.This())->what()).ToLocalChecked());
};

NAN_METHOD(AlertWrap::type) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<Integer>(AlertWrap::Unwrap(info.This())->type()));
};

NAN_METHOD(AlertWrap::message) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<String>(AlertWrap::Unwrap(info.This())->message()).ToLocalChecked());
};

NAN_METHOD(AlertWrap::category) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<Integer>(AlertWrap::Unwrap(info.This())->category()));
};

/*NAN_METHOD(AlertWrap::handle) {
    Nan::HandleScope scope;

    try {
        auto casted = getHandle <libtorrent::add_torrent_alert,
                             libtorrent::torrent_removed_alert,
                             libtorrent::save_resume_data_alert,
                             libtorrent::save_resume_data_failed_alert,
                             libtorrent::metadata_received_alert,
                             libtorrent::torrent_finished_alert> (*AlertWrap::Unwrap(info.This()));

        info.GetReturnValue().Set(TorrentHandleWrap::FromExisting(casted));
    } catch (std::exception e) {
        info.GetReturnValue().SetUndefined();
    }
};*/
