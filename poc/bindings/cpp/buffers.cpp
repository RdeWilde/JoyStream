#include "buffers.hpp"

namespace joystream {
namespace node {

uchar_vector NodeBufferToUCharVector(v8::Local<v8::Value> buffer) {
    return uchar_vector((const unsigned char *)::node::Buffer::Data(buffer), ::node::Buffer::Length(buffer));
}

uchar_vector StringToUCharVector(v8::Local<v8::Value> hex) {
    v8::String::Utf8Value value(hex);
    uchar_vector data = uchar_vector(std::string(*value));
    return data;
}

v8::Local<v8::Value> UCharVectorToNodeBuffer(uchar_vector &data) {
    auto buf = Nan::NewBuffer(data.size()).ToLocalChecked();
    auto pbuf = ::node::Buffer::Data(buf);
    //copyToArray should really have been marked const since it doesn't modify s_frk_state
    //to allow us to also pass in a const arg to UCharVectorToNodeBuffer
    data.copyToArray((unsigned char*)pbuf);
    return buf;
}

}}
