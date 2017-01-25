#include "buffers.hpp"
#include "libtorrent-node/utils.hpp"

namespace joystream {
namespace node {

uchar_vector NodeBufferToUCharVector(v8::Local<v8::Value> buffer) {
    if(!buffer->IsUint8Array()){
        throw std::runtime_error("argument is not a node buffer");
    }
    return uchar_vector((const unsigned char *)::node::Buffer::Data(buffer), ::node::Buffer::Length(buffer));
}

uchar_vector StringToUCharVector(v8::Local<v8::Value> value) {
    auto hex = ToNative<std::string>(value);
    uchar_vector data = uchar_vector(hex);
    return data;
}

v8::Local<v8::Object> UCharVectorToNodeBuffer(uchar_vector &data) {
    auto buf = Nan::NewBuffer(data.size()).ToLocalChecked();
    auto pbuf = ::node::Buffer::Data(buf);
    //copyToArray should really have been marked const since it doesn't modify s_frk_state
    //to allow us to also pass in a const arg to UCharVectorToNodeBuffer
    data.copyToArray((unsigned char*)pbuf);
    return buf;
}

}}
