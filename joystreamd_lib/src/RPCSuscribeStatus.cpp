#include <joystreamd_lib/RPCSuscribeStatus.hpp>

RPCSuscribeStatus::RPCSuscribeStatus(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet)
    : RPCRequestStreamServer(service, cq), wallet_(wallet)
{
    service_->RequestSuscribeStatus(&ctx_, &request_, &responder_, cq_, cq_, this);

}

void RPCSuscribeStatus::publishToClient(const joystream::bitcoin::SPVWallet::wallet_status_t status)
{
    joystream::daemon::rpc::WalletStatus response;

    std::cout << "STATUS BEING UPDATED !" << std::endl;

    response.set_status(joystream::daemon::rpc::WalletStatus_Status_UNINITIALIZED);

    // calling write seems to cancel the stream
    this->write(response);

    // If also calling finish it is ok
    //this->responder_.Finish(grpc::Status::OK, this);
}

void RPCSuscribeStatus::process()
{
    // Pop up a new instance for concurency
    new RPCSuscribeStatus(service_, cq_, wallet_);

    /*if (ctx_.IsCancelled()) {
        std::cout << "THE CALL IS CANCELLED" << std::endl;
        this->finish(true);
    }*/

    qRegisterMetaType<joystream::bitcoin::SPVWallet::wallet_status_t>("joystream::bitcoin::SPVWallet::wallet_status_t");

    QObject::connect(wallet_,  &joystream::bitcoin::SPVWallet::statusChanged,
                    this, &RPCSuscribeStatus::publishToClient);
}
