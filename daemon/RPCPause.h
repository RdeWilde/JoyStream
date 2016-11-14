#ifndef RPCPAUSE_H
#define RPCPAUSE_H

#include "RPCRequestNormal.h"
#include <core/Node.hpp>

class RPCPause : public RPCRequestNormal<joystream::daemon::rpc::Void> {
    public:
        RPCPause(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void onCall();

    private:
        joystream::core::Node* node_;
        joystream::daemon::rpc::Void request_;

};

#endif // RPCPAUSE_H
