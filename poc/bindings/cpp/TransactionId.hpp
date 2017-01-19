#ifndef JOYSTREAM_NODE_TRANSACTIONID_HPP
#define JOYSTREAM_NODE_TRANSACTIONID_HPP

#include <nan.h>

namespace Coin {
    class TransactionId;
}

namespace joystream {
namespace node {
namespace transactionid {

    /* @brief Creates node Buffer representing a raw Coin::TransactionId
     * @param {const Coin::TransactionId&}
     * @return {v8::Local<v8::Value>} node Buffer
     * @throws
     */
    v8::Local<v8::Value> toValue(const Coin::TransactionId &);

     /* @brief Converts a raw transaction id (in rpc byte order)
      * from a node buffer to Coin::TransactionId
      * @param {v8::Local<v8::Value>} node Buffer
      * @return {Coin::TransactionId}
      * @throws std::runtime_error if conversion fails
      */
     Coin::TransactionId fromValue(const v8::Local<v8::Value>&);
}}}
#endif
