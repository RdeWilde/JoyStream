#ifndef SERVERIMPL_H
#define SERVERIMPL_H

#include <core/Node.hpp>
#include <thread>
#include <grpc++/alarm.h>

#include "AsyncCallHandler.h"
#include "RPCRequest.h"

#include "protos/daemon.grpc.pb.h"


class ServerImpl final
{
    public:
        ServerImpl(joystream::core::Node* node);
        ~ServerImpl();
        void Init();
        void Run();

    private:
        joystream::core::Node *node_;
        std::unique_ptr<grpc::Server> server_;
        std::thread the_thread;
        std::unique_ptr<grpc::ServerCompletionQueue> cq_;
};

#endif // SERVERIMPL_H
