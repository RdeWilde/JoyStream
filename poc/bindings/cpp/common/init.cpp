#include <nan.h>
#include <addon/common/Transaction.hpp>

NAN_MODULE_INIT(InitCommon) {
    // name spacing object
    auto common = Nan::New<v8::Object>();
    target->Set(Nan::New("common").ToLocalChecked(), common);

    Transaction::Init(common);
}
