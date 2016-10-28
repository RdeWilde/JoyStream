#include "ServerImpl.h"

ServerImpl::ServerImpl(joystream::core::Node* node)
    : node_(node), the_thread()
{
    ServerImpl::Init();
}

ServerImpl::~ServerImpl()
{
    if(the_thread.joinable()) the_thread.join();
    // server_->Shutdown();
    std::cout << "Server destroyed" << std::endl;
    // Always shutdown the completion queue after the server.
    // cq_->Shutdown();
}

void ServerImpl::Init()
{
    the_thread = std::thread(&ServerImpl::Run,this);
}

void ServerImpl::Run()
{
    std::string server_address("0.0.0.0:3002");

    grpc::ServerBuilder builder;
    joystream::daemon::rpc::Daemon::AsyncService daemonService;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    builder.RegisterService(&daemonService);

    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();


    std::cout << "Server listening on " << server_address << std::endl;

    AsyncCallHandler handler(&daemonService, cq_.get(), node_);

    // run() is blocking
    handler.run();
}
