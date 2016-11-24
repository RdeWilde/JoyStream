#ifndef ASYNCCALLHANDLER_HPP
#define ASYNCCALLHANDLER_HPP


#include <grpc/grpc.h>
#include <grpc++/grpc++.h>
#include <core/Node.hpp>
#include <thread>

#include <joystreamd_lib/RPCRequest.hpp>

#include "protos/daemon.grpc.pb.h"

/**
 * @brief Basic class that handle all the rpcs calls
 * in a new thread
 */
class CompletionQueueDispatcher {

    public:
        CompletionQueueDispatcher();
        ~CompletionQueueDispatcher();
        void StartDispatcher(grpc::ServerCompletionQueue* cq);

    private:
        void run(grpc::ServerCompletionQueue* cq);
        std::thread thread_;

};
#endif // ASYNCCALLHANDLER_HPP
