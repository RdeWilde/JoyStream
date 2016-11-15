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
 *
 */
class CompletionQueueDispatcher {
    public:

        CompletionQueueDispatcher();
        ~CompletionQueueDispatcher();
        void run(grpc::ServerCompletionQueue* cq);

};
#endif // ASYNCCALLHANDLER_HPP
