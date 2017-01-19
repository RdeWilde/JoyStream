#ifndef JOYSTREAM_NODE_TRANSACTION_HPP
#define JOYSTREAM_NODE_TRANSACTION_HPP

#include <nan.h>

namespace Coin {
    class Transaction;
}

namespace joystream {
namespace node {
namespace transaction {

v8::Local<v8::Value> toValue(const Coin::Transaction &);
Coin::Transaction fromValue(const v8::Local<v8::Value>&);

}}}
#endif
