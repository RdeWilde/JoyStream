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

    std::cout << "Server listening on " << server_address << std::endl;

    // Initiate all the RPC methods declared in .proto file
    new RPCPause(&daemonService_, cq_.get(), node_);
    new RPCAddTorrent(&daemonService_, cq_.get(), node_);
    new RPCRemoveTorrent(&daemonService_, cq_.get(), node_);
    new RPCListTorrents(&daemonService_, cq_.get(), node_);
    new RPCPauseTorrent(&daemonService_, cq_.get(), node_);

    thread_ = std::thread(&CompletionQueueDispatcher::run,dispatcher_,cq_.get());

}
