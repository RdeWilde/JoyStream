#include <addon/util/buffers.hpp>

uchar_vector NodeBufferToUCharVector(v8::Local<v8::Value> buffer) {
    return uchar_vector((const unsigned char *)node::Buffer::Data(buffer), node::Buffer::Length(buffer));
}

uchar_vector StringToUCharVector(v8::Local<v8::Value> hex) {
    v8::String::Utf8Value value(hex);
    uchar_vector data = uchar_vector(std::string(*value));
    return data;
}

v8::Local<v8::Value> UCharVectorToNodeBuffer(uchar_vector &data) {
    Nan::EscapableHandleScope scope;
    auto buf = Nan::NewBuffer(data.size()).ToLocalChecked();
    auto pbuf = node::Buffer::Data(buf);
    data.copyToArray((unsigned char*)pbuf);
    return scope.Escape(buf);
}
