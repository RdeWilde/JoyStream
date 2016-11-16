#include <joystreamd_lib/ServerImpl.hpp>

ServerImpl::ServerImpl(joystream::core::Node* node, joystream::bitcoin::SPVWallet *wallet, QCoreApplication *app)
    : node_(node), wallet_(wallet), app_(app)
{
    Run();
}

ServerImpl::~ServerImpl()
{
    server_->Shutdown();
    std::cout << "Server destroyed" << std::endl;

    // Always shutdown the completion queue after the server.
    cq_->Shutdown();

    // Shutdown before pause() callback get called...
    node_->pause([this](){
        std::cout << "Node paused ready to quit application" << std::endl;
        app_->quit();
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
    new RPCPause(&daemonService_, cq_.get(), node_);
    new RPCAddTorrent(&daemonService_, cq_.get(), node_);
    new RPCRemoveTorrent(&daemonService_, cq_.get(), node_);
    new RPCListTorrents(&daemonService_, cq_.get(), node_);
    new RPCPauseTorrent(&daemonService_, cq_.get(), node_);
    new RPCStartTorrent(&daemonService_, cq_.get(), node_);

    // Initiate Wallet Service methods
    new RPCReceivedAddress(&walletService_, cq_.get(), wallet_);
    new RPCBalance(&walletService_, cq_.get(), wallet_);
    new RPCUnconfirmedBalance(&walletService_, cq_.get(), wallet_);

    thread_ = std::thread(&CompletionQueueDispatcher::run,dispatcher_,cq_.get());

}
