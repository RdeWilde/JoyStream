#include <string>
#include <nan.h>
#include <addon/util/buffers.hpp>
#include <addon/util/helpers.hpp>

namespace joystream {
namespace addon {
namespace util {

v8::Local<v8::Value> GetBufferByKey(const v8::Local<v8::Value>& value, const std::string &key) {
    Nan::EscapableHandleScope scope;
    auto obj = Nan::To<v8::Object>(value);
    auto buf = Nan::Get(obj.ToLocalChecked(), Nan::New(key).ToLocalChecked()).ToLocalChecked();
    if(!buf->IsUint8Array()) {
        throw std::runtime_error("Invalid Object");
    }
    return scope.Escape(buf);
}

v8::Local<v8::Value> UCharVectorToObject(uchar_vector& data, const std::string &key) {
    Nan::EscapableHandleScope scope;
    auto buffer = util::UCharVectorToNodeBuffer(data);
    auto obj = Nan::New<v8::Object>();
    Nan::Set(obj, Nan::New(key).ToLocalChecked(), buffer);
    return scope.Escape(obj);
}

}}}