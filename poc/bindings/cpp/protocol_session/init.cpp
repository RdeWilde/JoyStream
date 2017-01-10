#include <nan.h>
#include <addon/protocol_session/ConnectionTCPEndpoint.hpp>

using namespace joystream::addon::protocol_session;

NAN_MODULE_INIT(InitProtocolSession) {

    auto protocolSession = Nan::New<v8::Object>();
    target->Set(Nan::New("protocolSession").ToLocalChecked(), protocolSession);

    ConnectionTCPEndpoint::Init(protocolSession);
}
