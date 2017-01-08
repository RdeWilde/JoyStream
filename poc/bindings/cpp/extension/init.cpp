#include <nan.h>
#include <addon/extension/TorrentPluginStatus.hpp>

NAN_MODULE_INIT(InitExtension) {

    auto extension = Nan::New<v8::Object>();
    target->Set(Nan::New("extension").ToLocalChecked(), extension);

    PeerPluginStatus::Init(extension);
    TorrentPluginStatus::Init(extension);
}
