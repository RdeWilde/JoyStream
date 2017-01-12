#include <nan.h>
#include <addon/extension/TorrentPluginStatus.hpp>
#include <addon/extension/PeerPluginStatus.hpp>

using namespace joystream::addon::extension;

NAN_MODULE_INIT(InitExtension) {

    auto extension = Nan::New<v8::Object>();
    target->Set(Nan::New("extension").ToLocalChecked(), extension);

    TorrentPluginStatus::Init(extension);
    PeerPluginStatus::Init(extension);
}
