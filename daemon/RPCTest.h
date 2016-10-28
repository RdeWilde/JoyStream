#ifndef RPCTEST_H
#define RPCTEST_H

#include "protos/daemon.grpc.pb.h"
#include "protos/daemon.pb.h"
#include "RPCRequest.h"

class RPCTest : public RPCRequest {
    public:
        RPCTest(joystream::daemon::rpc::Daemon::AsyncService * service, grpc::ServerCompletionQueue* cq);
        void proceed(bool fok, void * tag);

    private:
        joystream::daemon::rpc::Daemon::AsyncService* service_;
        grpc::ServerCompletionQueue* cq_;

        joystream::daemon::rpc::TestResponce responce_;
        grpc::ServerAsyncResponseWriter<joystream::daemon::rpc::TestResponce> responder_;
        joystream::daemon::rpc::TestRequest request_;
        grpc::ServerContext ctx_;
        CallStatus status_;
};
#endif // RPCTEST_H
