#ifndef JOYSTREAM_NODE_TRANSACTIONID_HPP
#define JOYSTREAM_NODE_TRANSACTIONID_HPP

#include <nan.h>

namespace Coin {
    class TransactionId;
}

namespace joystream {
namespace node {
namespace transactionid {

v8::Local<v8::Value> toValue(const Coin::TransactionId &);
Coin::TransactionId fromValue(const v8::Local<v8::Value>&);

}}}
#endif
