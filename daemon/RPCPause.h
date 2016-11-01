#ifndef RPCPAUSE_H
#define RPCPAUSE_H

#include "protos/daemon.grpc.pb.h"
#include "protos/daemon.pb.h"
#include "RPCRequest.h"
#include <core/Node.hpp>

class RPCPause : public RPCRequest {
    public:
        RPCPause(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void proceed(bool fok);

    private:
        joystream::daemon::rpc::Daemon::AsyncService* service_;
        grpc::ServerCompletionQueue* cq_;
        joystream::core::Node* node_;

        joystream::daemon::rpc::Void response_;
        grpc::ServerAsyncResponseWriter<joystream::daemon::rpc::Void> responder_;
        joystream::daemon::rpc::Void request_;
        grpc::ServerContext ctx_;
        CallStatus status_;

};

#endif // RPCPAUSE_H
