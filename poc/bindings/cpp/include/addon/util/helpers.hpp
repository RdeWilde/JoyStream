#ifndef JOYSTREAM_NODE_ADDON_COMMON_TEMPLATES_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_TEMPLATES_HPP

#include <string>
#include <nan.h>
#include <addon/util/buffers.hpp>

namespace joystream {
namespace addon {
namespace util {

v8::Local<v8::Value> GetBufferByKey(const v8::Local<v8::Value>& value, const std::string &key);

template<class T>
T GetNativeTypeFromBufferByKey(const v8::Local<v8::Value>& value, const std::string &key) {
    auto buf = GetBufferByKey(value, key);
    return T(util::NodeBufferToUCharVector(buf));
}

v8::Local<v8::Value> UCharVectorToObject(uchar_vector& data, const std::string &key);

}}}
#endif
