#include "ServerImpl.h"

ServerImpl::ServerImpl(joystream::core::Node* node)
    : node_(node)
{
    Run();
}

ServerImpl::~ServerImpl()
{
    server_->Shutdown();
    std::cout << "Server destroyed" << std::endl;
    // Always shutdown the completion queue after the server.
    cq_->Shutdown();
}


void ServerImpl::Run()
{
    std::string server_address("0.0.0.0:3002");

    grpc::ServerBuilder builder;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    builder.RegisterService(&daemonService_);

    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();

    handler_.setCompletionQueue(cq_.get());

    std::cout << "Server listening on " << server_address << std::endl;

    // Initiate all the RPC methods declared in .proto file
    new RPCTest(&daemonService_, cq_.get());
    new RPCPause(&daemonService_, cq_.get(), node_);


}
