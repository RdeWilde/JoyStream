#ifndef RPCPAUSE_HPP
#define RPCPAUSE_HPP

#include <daemon/RPCRequestNormal.hpp>
#include <core/Node.hpp>

class RPCPause : public RPCRequestNormal<joystream::daemon::rpc::Void, joystream::daemon::rpc::Void> {
    public:
        RPCPause(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void process();

    private:
        joystream::core::Node* node_;
};

#endif // RPCPAUSE_HPP
