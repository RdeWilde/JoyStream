#ifndef RPCPAUSE_HPP
#define RPCPAUSE_HPP

#include "protos/daemon.grpc.pb.h"
#include "protos/daemon.pb.h"

#include <daemon/RPCRequestNormal.hpp>
#include <core/Node.hpp>

class RPCPause : public RPCRequestNormal<joystream::daemon::rpc::Void> {
    public:
        RPCPause(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void onCall();

    private:
        joystream::core::Node* node_;
        joystream::daemon::rpc::Void request_;

};

#endif // RPCPAUSE_HPP
