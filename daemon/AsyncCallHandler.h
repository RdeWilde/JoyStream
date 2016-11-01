#ifndef ASYNCCALLHANDLER_H
#define ASYNCCALLHANDLER_H


#include <grpc/grpc.h>
#include <grpc++/grpc++.h>
#include <core/Node.hpp>
#include <thread>

#include "RPCRequest.h"
#include "RPCTest.h"
#include "RPCPause.h"

#include "protos/daemon.grpc.pb.h"

/**
 * @brief Basic class that handle all the rpcs calls
 *
 */
class AsyncCallHandler {
    public:

        AsyncCallHandler();
        ~AsyncCallHandler();
        void setCompletionQueue(grpc::ServerCompletionQueue* cq);
        void run();

    private:
        grpc::ServerCompletionQueue* cq_;
        std::thread thread_;

};
#endif // ASYNCCALLHANDLER_H
