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
class CompletionQueueDispatcher {
    public:

        CompletionQueueDispatcher();
        ~CompletionQueueDispatcher();
        void run(grpc::ServerCompletionQueue* cq);

};
#endif // ASYNCCALLHANDLER_H
