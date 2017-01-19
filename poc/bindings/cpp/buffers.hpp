#ifndef JOYSTREAM_NODE_BUFFERS_HPP
#define JOYSTREAM_NODE_BUFFERS_HPP

#include <nan.h>
#include <stdutils/uchar_vector.h>

namespace joystream {
namespace node {

uchar_vector NodeBufferToUCharVector(v8::Local<v8::Value>);
uchar_vector StringToUCharVector(v8::Local<v8::Value>);

v8::Local<v8::Value> UCharVectorToNodeBuffer(uchar_vector&);

template<class T>
v8::Local<v8::Value> UCharVectorBasedToV8Value(const T &t) {
    uchar_vector data = t.toUCharVector();
    auto buf = UCharVectorToNodeBuffer(data);
    return buf;
}

template<class T>
T fromV8ValueToUCharVectorBased(const v8::Local<v8::Value>& buffer) {
    uchar_vector data = NodeBufferToUCharVector(buffer);
    return T(data);
}

}}

#endif
