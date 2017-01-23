/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 12 2016
 */
 
#ifndef JOYSTREAM_NODE_ADDON_UTILS_HPP
#define JOYSTREAM_NODE_ADDON_UTILS_HPP

#include <nan.h>

/**
* Guard, used in the context of a function call callback to a constructor function,
* in order to detect absence of javascript `new` operator. Absence leads to reinvocation
* of provided constructor. A macro is used in order to be able to return from callback.
*
* It is meant to mimic the effect of the following snippet in a javascript constructor function named `T`
*
* if(!(this instanceof T))
*   return new T(args);
*/

#define NEW_OPERATOR_GUARD(info, ctr) \
if(!info.IsConstructCall()) { \
  std::vector<v8::Local<v8::Value>> argv; \
  for(int i = 0;i < info.Length();i++) \
    argv.push_back(info[i]); \
  v8::Local<v8::Function> cons = Nan::New(ctr); \
  info.GetReturnValue().Set(cons->NewInstance(Nan::GetCurrentContext(), info.Length(), &argv[0]).ToLocalChecked()); \
  return; \
} \

/**
 * Object setters.
 * @param {v8::Local<v8::Object>} o
 * @param {const char *| const std::string &} key name
 */

// @param {v8::Local<v8::Value>} val
#define SET_VAL(o, key, val)    (Nan::Set(o, Nan::New(key).ToLocalChecked(), val))

#define SET_UNDEFINED(o, key)   (SET_VAL(o, key, Nan::Undefined()))

// @param {int} val
// Based on v8::Local<v8::Int32> Nan::New<T>(int32_t value)
#define SET_INT32(o, key, val)    (SET_VAL(o, key, Nan::New(val)))

// @param {int} val
// Based on v8::Local<v8::Uint32> Nan::New<T>(uint32_t value)
#define SET_UINT32(o, key, val)   (SET_VAL(o, key, Nan::New(val)))

// @param {double} val
// Based on v8::Local<v8::Number> Nan::New<T>(double value)
#define SET_DOUBLE(o, key, val) (SET_VAL(o, key, Nan::New(val)))

// @param {bool} val
// Based on v8::Local<v8::Boolean> Nan::New<T>(bool value);
#define SET_BOOL(o, key, val)   (SET_VAL(o, key, Nan::New(val)))

// @param {const char * | const std::string &} val
// Based on Nan::MaybeLocal<v8::String> Nan::New<T>(const char * value)
#define SET_CONST_CHAR(o, key, val)    (Nan::Set(o, Nan::New(key).ToLocalChecked(), Nan::New(val).ToLocalChecked()))

// @param {const char * | const std::string &} val
// Based on Nan::MaybeLocal<v8::String> Nan::New<T>(std::string const& value);
#define SET_STD_STRING(o, key, val)    (Nan::Set(o, Nan::New(key).ToLocalChecked(), Nan::New(val).ToLocalChecked()))

/**
 * @brief Convert from v8 to standard string.
 * @param v v8 string
 * @return standard string
 */
std::string toString(const v8::Local<v8::String> & v);

/**
 * @brief Get value from object with given key name.
 * @param {v8::Local<v8::Object>} o
 * @param {const std::string &} keyName
 * @throws std::runtime_error if key could not be recovered
 * @return v8::Local<v8::Value> value
 */
v8::Local<v8::Value> GetValue(const v8::Local<v8::Object> & o, const std::string & keyName);

/**
 * @brief Convert value to given native type instance
 */
template<class T>
T To(const v8::Local<v8::Value> & val) {

  // // Native types conversions
  // https://github.com/nodejs/nan/blob/master/doc/converters.md#api_nan_to
  // Nan::Maybe<bool> Nan::To<bool>(v8::Local<v8::Value> val);
  // Nan::Maybe<double> Nan::To<double>(v8::Local<v8::Value> val);
  // Nan::Maybe<int32_t> Nan::To<int32_t>(v8::Local<v8::Value> val);
  // Nan::Maybe<int64_t> Nan::To<int64_t>(v8::Local<v8::Value> val);
  // Nan::Maybe<uint32_t> Nan::To<uint32_t>(v8::Local<v8::Value> val);

  Nan::Maybe<T> maybeT = Nan::To<T>(val);

  if(!maybeT.IsJust())
    throw std::runtime_error("Value malformed, conversion failed.");
  else
    return maybeT.FromJust();
}

// We have to specialize for std::string, as To returns MaybeLocal, not Maybe as above.
template<>
std::string To(const v8::Local<v8::Value> & val);

/**
 * Object getters.
 * @param {v8::Local<v8::Object>} o
 * @param {const std::string &} key name
 */

// @return {v8::Local<v8::Value>}
#define GET_VAL(o, key)           (GetValue(o, key))

// @return {int32_t}
#define GET_INT32(o, key)         (To<int32_t>(GET_VAL(o, key)))

// @return {uint32_t}
#define GET_UINT32(o, key)        (To<uint32_t>(GET_VAL(o, key)))

// @return {int64_t}
#define GET_INT64(o, key)         (To<int64_t>(GET_VAL(o, key)))

// @return {double}
#define GET_DOUBLE(o, key)        (To<double>(GET_VAL(o, key)))

// @return {bool}
#define GET_BOOL(o, key)          (To<bool>(GET_VAL(o, key)))

// @return {std::string}
#define GET_STD_STRING(o, key)    (To<std::string>(GET_VAL(o, key)))

///////////////////////////////////////////////////////////////////////////////
/// Utility macros from @fanatid in macro.hpp@a4901b
///////////////////////////////////////////////////////////////////////////////

#define REQUIRE_ARGUMENTS(n)                                                  \
  if (info.Length() < (n)) {                                                  \
    return Nan::ThrowTypeError("Expected " #n "arguments");                   \
  }

#define ARGUMENTS_OPTIONAL_OBJECT(i, var, defaultValue)                       \
  v8::Local<v8::Object> var;                                                  \
  if (info.Length() > i && !info[i]->IsUndefined()) {                         \
    if (!info[i]->IsObject()) {                                               \
      return Nan::ThrowTypeError("Argument " #i " must be a Object");         \
    }                                                                         \
    var = info[i]->ToObject();                                                \
  } else {                                                                    \
    var = (defaultValue);                                                     \
  }

#define ARGUMENTS_IS_NUMBER(i)                                                \
  (info.Length() > (i) && info[i]->IsNumber())

#define ARGUMENTS_REQUIRE_NUMBER(i, var)                                      \
  if (!ARGUMENTS_IS_NUMBER(i)) {                                              \
    return Nan::ThrowTypeError("Argument " #i " must be a Number");           \
  }                                                                           \
  int64_t var = info[i]->IntegerValue();

#define ARGUMENTS_IS_BOOLEAN(i)                                               \
  (info.Length() > (i) && info[i]->IsBoolean())

#define ARGUMENTS_REQUIRE_BOOLEAN(i, var)                                     \
  if (!ARGUMENTS_IS_BOOLEAN(i)) {                                             \
    return Nan::ThrowTypeError("Argument " #i " must be a Boolean");          \
  }                                                                           \
  bool var = info[i]->BooleanValue();

#define ARGUMENTS_IS_STRING(i)                                                \
  (info.Length() > (i) && info[i]->IsString())

#define ARGUMENTS_REQUIRE_STRING(i, var)                                      \
  if (!ARGUMENTS_IS_STRING(i)) {                                              \
    return Nan::ThrowTypeError("Argument " #i " must be a String");           \
  }                                                                           \
  Nan::Utf8String var(info[i]);

#define ARGUMENTS_IS_FUNCTION(i)                                              \
  (info.Length() > (i) && info[i]->IsFunction())

#define ARGUMENTS_REQUIRE_FUNCTION(i, var)                                    \
  if (!ARGUMENTS_IS_FUNCTION(i)) {                                            \
    return Nan::ThrowTypeError("Argument " #i " must be a Function");         \
  }                                                                           \
  v8::Local<v8::Function> var = v8::Local<v8::Function>::Cast(info[i]);

#define ARGUMENTS_IS_OBJECT(i)                                               \
  (info.Length() >= (i) && info[i]->IsObject())

#define ARGUMENTS_IS_INSTANCE(i, cls)                                         \
  (ARGUMENTS_IS_OBJECT(i) && Nan::New(cls::prototype)->HasInstance(info[i]))

#define ARGUMENTS_REQUIRE_INSTANCE(i, cls, var)                               \
  if (!ARGUMENTS_IS_INSTANCE(i, cls)) {                                       \
    return Nan::ThrowTypeError("Argument " #i " must be a " #cls);            \
  }                                                                           \
  cls* var = Nan::ObjectWrap::Unwrap<cls>(info[i]->ToObject());

///////////////////////////////////

// Recover object wrapped on this object
#define THIS(cls)                                                             \
  Nan::ObjectWrap::Unwrap<cls>(info.This())                                   \

// Recover type from object encoding
#define ARGUMENTS_REQUIRE_OBJECT(i, type, var)                                \
  type var;                                                                   \
  try {                                                                       \
    var = fromValue(info[i]);                                                 \
  } catch (const std::runtime_error & e) {                                    \
    std::string m("Argument " #i " is not a " #type " object: ");             \
    return Nan::ThrowTypeError(m + e.what());                                 \
  }

#endif
