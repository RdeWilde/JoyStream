#ifndef JOYSTREAM_NODE_OUTPOINT_HPP
#define JOYSTREAM_NODE_OUTPOINT_HPP

#include <nan.h>

namespace Coin {
    class typesafeOutPoint;
}

namespace joystream {
namespace node {
namespace outpoint {

    /** Creates JavaScript Object representing a Coin::typesafeOutPoint
      * {
      *   txid: Buffer,  // from transactionid::toValue
      *   index: Number
      * }
    */
    v8::Local<v8::Value> toValue(const Coin::typesafeOutPoint &);

    /* Converts a JavaScript Object to a native Coin::typesafeOutPoint
     * Expects an object of the form:
     * {
     *   txid: Buffer,
     *   index: Number
     * }
     */
    Coin::typesafeOutPoint fromValue(const v8::Local<v8::Value>&);

}}}
#endif
