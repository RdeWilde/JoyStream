#include <nan.h>
#include <addon/extension/TorrentPluginStatus.hpp>
#include <addon/extension/PeerPluginStatus.hpp>

NAN_MODULE_INIT(InitExtension) {

    auto extension = Nan::New<v8::Object>();
    target->Set(Nan::New("extension").ToLocalChecked(), extension);

    joystream::addon::extension::PeerPluginStatus::Init(extension);
    joystream::addon::extension::TorrentPluginStatus::Init(extension);
}
