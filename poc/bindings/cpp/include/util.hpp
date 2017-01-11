/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 9 2017
 */

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
