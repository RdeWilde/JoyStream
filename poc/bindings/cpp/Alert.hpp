/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include <nan.h>

#define ALERT_WRAPPER_IMPL(name) \
  virtual int type() const override { return name::alert_type; } \
  virtual int category() const override { return name::static_category; } \
  virtual char const* what() const override { return #name; }

namespace joystream {
namespace node_addon {

/**
 * @brief Base class for all alert binding classes
 *
 * Is not exposed to javascript itself, hence one cannot
 * create an alert object in javascript, or even check if
 * an existing object is an instance of this class.
 */
class Alert : public Nan::ObjectWrap {

public:

  static NAN_METHOD(type);
  static NAN_METHOD(what);
  static NAN_METHOD(message);
  static NAN_METHOD(category);

protected:

  static void Init(const std::string & className,
                   v8::Local<v8::FunctionTemplate> & t,
                   int internalFieldCount = 1);

  virtual int type() const = 0;
  virtual char const* what() const = 0;
  virtual std::string message() const = 0;
  virtual int category() const = 0;

};

}
}
