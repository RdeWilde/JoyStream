#ifndef JOYSTREAM_NODE_ADDON_COMMON_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_HPP

#include <string>
#include <nan.h>
#include <addon/util/buffers.hpp>

namespace joystream {
namespace addon {
namespace common {

template<class T>
v8::Local<v8::Value> UCharVectorBasedToV8Value(const T &t) {
    Nan::EscapableHandleScope scope;
    uchar_vector data = t.toUCharVector();
    auto buf = util::UCharVectorToNodeBuffer(data);
    return scope.Escape(buf);
}

template<class T>
T fromV8ValueToUCharVectorBased(const v8::Local<v8::Value>& buffer) {
    uchar_vector data = util::NodeBufferToUCharVector(buffer);
    return T(data);
}


}}}
#endif
