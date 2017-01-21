#ifndef JOYSTREAM_NODE_PUBKEYHASH_HPP
#define JOYSTREAM_NODE_PUBKEYHASH_HPP

#include <nan.h>
#include <common/PubKeyHash.hpp>

namespace joystream {
namespace node {
namespace pubkeyhash {

    /* @brief Creates node Buffer representing a raw Coin::PubKeyHash
     * @param {const Coin::PubKeyHash&}
     * @return {v8::Local<v8::Value>} node Buffer
     * @throws
     */
    v8::Local<v8::Value> toValue(const Coin::PubKeyHash &);

     /* @brief Converts a raw pubkeyhash from a node buffer to Coin::PubKeyHash
      * @param {v8::Local<v8::Value>} node Buffer
      * @return {Coin::PubKeyHash}
      * @throws std::runtime_error if conversion fails
      */
     Coin::PubKeyHash fromValue(const v8::Local<v8::Value>&);

}}}
#endif
