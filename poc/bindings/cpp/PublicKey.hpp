#ifndef JOYSTREAM_NODE_PUBLICKEY_HPP
#define JOYSTREAM_NODE_PUBLICKEY_HPP

#include <nan.h>

namespace Coin {
    class PublicKey;
}

namespace joystream {
namespace addon {
namespace common {
namespace publickey {

    /* @brief Creates node Buffer representing a raw Coin::PublicKey
     * @param {const Coin::PublicKey&}
     * @return {v8::Local<v8::Value>} node Buffer
     * @throws
     */
    v8::Local<v8::Value> toValue(const Coin::PublicKey &);

     /* @brief Converts a raw public key from a node buffer to Coin::PublicKey
      * @param {v8::Local<v8::Value>} node Buffer
      * @return {Coin::PublicKey}
      * @throws std::runtime_error if conversion fails
      */
     Coin::PublicKey fromValue(const v8::Local<v8::Value>&);
}}}}
#endif
