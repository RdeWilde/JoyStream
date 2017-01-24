#include "payment_channel.hpp"
#include "PublicKey.hpp"

#include "utils.hpp"
#include "buffers.hpp"

#include <paymentchannel/Commitment.hpp>
#include <CoinCore/CoinNodeData.h>

namespace joystream {
namespace node {

  #define VALUE_KEY "value"
  #define PAYOR_KEY "payor"
  #define PAYEE_KEY "payee"
  #define LOCKTIME_KEY "locktime"

  NAN_MODULE_INIT(InitPaymentChannel) {

    Nan::Set(target, Nan::New("commitmentToOutput").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(CommitmentToOutput)->GetFunction());
  }

  NAN_METHOD(CommitmentToOutput) {
    if(info.Length() < 1) {
      Nan::ThrowTypeError("missing commitment argument");
      return;
    }

    try {
      paymentchannel::Commitment commitment = decode(info[0]);
      auto txout = commitment.contractOutput();
      auto rawoutput = txout.getSerialized();
      info.GetReturnValue().Set(UCharVectorToNodeBuffer(rawoutput));
    } catch(std::exception &e) {
      Nan::ThrowTypeError(e.what());
    }

  }

  paymentchannel::Commitment decode(const v8::Local<v8::Value> &commitment) {
    if(!commitment->IsObject()){
        throw std::runtime_error("argument not an Object");
    }

    auto obj = Nan::To<v8::Object>(commitment).ToLocalChecked();
    GET_VAL(obj, VALUE_KEY);
    GET_VAL(obj, LOCKTIME_KEY);
    auto value = GET_INT64(obj, VALUE_KEY); // Number satoshi
    auto payor = GET_VAL(obj, PAYOR_KEY); // public_key
    auto payee = GET_VAL(obj, PAYEE_KEY); // public_key
    auto locktime = GET_INT32(obj, LOCKTIME_KEY); // Number locktime counter
std::cout << "locktime:" << locktime << std::endl;
    return paymentchannel::Commitment(value,
                                      public_key::decode(payor),
                                      public_key::decode(payee),
                                      //relative_locktime::decode(locktime)); //todo
                                      Coin::RelativeLockTime::fromBlockUnits(locktime));
  }

}}
