#ifndef ASYNCCALLHANDLER_H
#define ASYNCCALLHANDLER_H


#include <grpc/grpc.h>
#include <grpc++/grpc++.h>
#include <core/Node.hpp>

#include "RPCRequest.h"
#include "RPCTest.h"
#include "RPCPause.h"

#include "protos/daemon.grpc.pb.h"


class AsyncCallHandler {
    public:

        AsyncCallHandler(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        ~AsyncCallHandler();
        void run();

    private:
        joystream::daemon::rpc::Daemon::AsyncService* service_;
        grpc::ServerCompletionQueue* cq_;

};
#endif // ASYNCCALLHANDLER_H
