#include <joystreamd_lib/ServerImpl.hpp>

ServerImpl::ServerImpl(joystream::appkit::AppKit* kit, QCoreApplication *app, std::string port)
    : kit_(kit), app_(app)
{
    Run(port);
}

void ServerImpl::Shutdown()
{

    std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(1000);

    // Adding deadline so server will cancel server side streaming request
    // Without the deadline, request that already started (called send by the client )
    // will wait until they are finished which will block the queue.
    server_->Shutdown(deadline);
    std::cout << "Server Shutting Done" << std::endl;

    // Always shutdown the completion queue after the server.
    cq_->Shutdown();

    kit_->shutdown([this](){
        std::cout << "Stopping..."<< std::endl;
        this->app_->quit();
    });

}

void ServerImpl::Run(std::string port)
{
    std::stringstream ss;
    ss << "0.0.0.0:" << port;
    std::string server_address = ss.str();

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
    new RPCSuscribeEvents(&daemonService_, cq_.get(), kit_->node());
    new RPCGetTorrentState(&daemonService_, cq_.get(), kit_->node());
    new RPCBuyTorrent(&daemonService_, cq_.get(), kit_);
    new RPCSellTorrent(&daemonService_, cq_.get(), kit_);

    // Initiate Wallet Service methods
    new RPCReceivedAddress(&walletService_, cq_.get(), kit_->wallet());
    new RPCBalance(&walletService_, cq_.get(), kit_->wallet());
    new RPCUnconfirmedBalance(&walletService_, cq_.get(), kit_->wallet());
    new RPCStatus(&walletService_, cq_.get(), kit_->wallet());
    new RPCSuscribeStatus(&walletService_, cq_.get(), kit_->wallet());

    dispatcher_.StartDispatcher(cq_.get());

}
