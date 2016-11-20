#include <joystreamd_lib/ServerImpl.hpp>

ServerImpl::ServerImpl(joystream::appkit::AppKit* kit, QCoreApplication *app)
    : kit_(kit), app_(app)
{
    Run();
}

void ServerImpl::Shutdown()
{
    server_->Shutdown();
    std::cout << "Server destroyed" << std::endl;

    // Always shutdown the completion queue after the server.
    cq_->Shutdown();

    // Shutdown but doesn't show 'Stopping...'
    kit_->shutdown([this](){
        std::cout << "Stopping..."<< std::endl;
        this->app_->quit();
        delete this;
    });
}

void ServerImpl::Run()
{
    std::string server_address("0.0.0.0:3002");

    grpc::ServerBuilder builder;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    builder.RegisterService(&daemonService_);
    builder.RegisterService(&walletService_);

    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();

    std::cout << "Server listening on " << server_address << std::endl;

    // Initiate Daemon Service methods
    new RPCPause(&daemonService_, cq_.get(), kit_->node());
    new RPCAddTorrent(&daemonService_, cq_.get(), kit_->node());
    new RPCRemoveTorrent(&daemonService_, cq_.get(), kit_->node());
    new RPCListTorrents(&daemonService_, cq_.get(), kit_->node());
    new RPCPauseTorrent(&daemonService_, cq_.get(), kit_->node());
    new RPCStartTorrent(&daemonService_, cq_.get(), kit_->node());

    // Initiate Wallet Service methods
    new RPCReceivedAddress(&walletService_, cq_.get(), kit_->wallet());
    new RPCBalance(&walletService_, cq_.get(), kit_->wallet());
    new RPCUnconfirmedBalance(&walletService_, cq_.get(), kit_->wallet());
    new RPCStatus(&walletService_, cq_.get(), kit_->wallet());
    new RPCSuscribeStatus(&walletService_, cq_.get(), kit_->wallet());


    thread_ = std::thread(&CompletionQueueDispatcher::run,dispatcher_,cq_.get());

}
