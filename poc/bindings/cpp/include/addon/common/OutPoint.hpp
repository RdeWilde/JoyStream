#ifndef JOYSTREAM_NODE_ADDON_COMMON_OUTPOINT_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_OUTPOINT_HPP

#include <nan.h>
#include <common/typesafeOutPoint.hpp>

namespace joystream {
namespace addon {
namespace common {

class OutPoint: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);

      static v8::Local<v8::Object> NewInstance(const Coin::typesafeOutPoint&);
      static bool IsInstance(v8::Object&);

      Coin::typesafeOutPoint outpoint() const;

    private:
      Coin::typesafeOutPoint _outpoint;
      static Nan::Persistent<v8::Function> constructor;

      static NAN_METHOD(New);

};

}}}
#endif
