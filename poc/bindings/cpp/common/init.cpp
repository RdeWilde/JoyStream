#include <nan.h>
#include <addon/common/Transaction.hpp>
#include <addon/common/PrivateKey.hpp>
#include <addon/common/PubKeyHash.hpp>
#include <addon/common/PublicKey.hpp>
#include <addon/common/TransactionId.hpp>
#include <addon/common/OutPoint.hpp>

NAN_MODULE_INIT(InitCommon) {
    // name spacing object
    auto common = Nan::New<v8::Object>();
    target->Set(Nan::New("common").ToLocalChecked(), common);

    joystream::addon::common::Transaction::Init(common);
    joystream::addon::common::PrivateKey::Init(common);
    joystream::addon::common::PubKeyHash::Init(common);
    joystream::addon::common::PublicKey::Init(common);
    joystream::addon::common::TransactionId::Init(common);
    joystream::addon::common::OutPoint::Init(common);
}
