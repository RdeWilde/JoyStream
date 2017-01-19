#ifndef JOYSTREAM_NODE_BUFFERS_HPP
#define JOYSTREAM_NODE_BUFFERS_HPP

#include <nan.h>
#include <stdutils/uchar_vector.h>

namespace joystream {
namespace node {

// Utility Methods to convert between node Buffer and uchar_vector

/**
 * @brief Copies data from a node Buffer into a new uchar_vector
 * @param {v8::Local<v8::Value>} value
 * @throws std::runtime_error if value not a node buffer
 * @return uchar_vector
 */
uchar_vector NodeBufferToUCharVector(v8::Local<v8::Value>);

/**
 * @brief Converts hex encoded data into a uchar_vector
 * @param {v8::Local<v8::Value>} value
 * @throws std::runtime_error if value not a string
 * @return uchar_vector
 */
 uchar_vector StringToUCharVector(v8::Local<v8::Value>);

/**
 * @brief Copies data from a uchar_vector into a new node Buffer
 * @param uchar_vector data
 * @return {v8::Local<v8::Value>} node Buffer
 */
v8::Local<v8::Value> UCharVectorToNodeBuffer(uchar_vector&);


/* @brief Convert types derived from Coin::UCharArray<> to a Node Buffer.
 * @param  {Coin::UCharArray<>&} data
 * @return {v8::Local<v8::Value>} node Buffer
 */
template<class T>
v8::Local<v8::Value> UCharVectorBasedToV8Value(const T &t) {
    uchar_vector data = t.toUCharVector();
    auto buf = UCharVectorToNodeBuffer(data);
    return buf;
}

/* Converts a node Buffer to a native type that can be constructed from
 * a uchar_vector.
 */
template<class T>
T V8ValueToUCharVectorBased(const v8::Local<v8::Value>& buffer) {
    uchar_vector data = NodeBufferToUCharVector(buffer);
    return T(data);
}

}}

#endif
