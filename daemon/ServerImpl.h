#ifndef SERVERIMPL_H
#define SERVERIMPL_H

#include <core/Node.hpp>
#include <grpc++/alarm.h>

#include "CompletionQueueDispatcher.h"
#include "RPCRequest.h"

#include "protos/daemon.grpc.pb.h"


class ServerImpl final
{
    public:
        ServerImpl(joystream::core::Node* node);
        ~ServerImpl();
        void Run();

    private:
        joystream::daemon::rpc::Daemon::AsyncService daemonService_;
        joystream::core::Node *node_;
        std::unique_ptr<grpc::Server> server_;
        std::unique_ptr<grpc::ServerCompletionQueue> cq_;
        CompletionQueueDispatcher dispatcher_;
};

#endif // SERVERIMPL_H
