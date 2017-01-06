#ifndef JOYSTREAM_NODE_ADDON_BUFFERS_HPP
#define JOYSTREAM_NODE_ADDON_BUFFERS_HPP

#include <nan.h>
#include <stdutils/uchar_vector.h>

uchar_vector NodeBufferToUCharVector(v8::Local<v8::Value>);
uchar_vector StringToUCharVector(v8::Local<v8::Value>);

v8::Local<v8::Value> UCharVectorToNodeBuffer(uchar_vector&);

#endif
