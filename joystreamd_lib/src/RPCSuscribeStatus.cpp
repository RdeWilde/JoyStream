#include <joystreamd_lib/RPCSuscribeStatus.hpp>

RPCSuscribeStatus::RPCSuscribeStatus(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet)
    : RPCRequestStreamServer(service, cq), wallet_(wallet)
{
    ctx_.AsyncNotifyWhenDone(this);
    service_->RequestSuscribeStatus(&ctx_, &request_, &responder_, cq_, cq_, this);

}


void RPCSuscribeStatus::publishToClient(const joystream::bitcoin::SPVWallet::wallet_status_t status)
{
    joystream::daemon::rpc::WalletStatus response;

    response.set_status(joystream::daemon::rpc::WalletStatus_Status_UNINITIALIZED);

    this->write(response);
}

void RPCSuscribeStatus::process()
{
    // Pop up a new instance for concurency
    new RPCSuscribeStatus(service_, cq_, wallet_);

    // Still need this ?
    qRegisterMetaType<joystream::bitcoin::SPVWallet::wallet_status_t>("joystream::bitcoin::SPVWallet::wallet_status_t");

    QObject::connect(wallet_,  &joystream::bitcoin::SPVWallet::statusChanged,
                    this, &RPCSuscribeStatus::publishToClient);
}
