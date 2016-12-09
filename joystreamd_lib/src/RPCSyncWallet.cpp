#include <joystreamd_lib/RPCSyncWallet.hpp>


RPCSyncWallet::RPCSyncWallet(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::appkit::AppKit* appKit)
    : RPCRequestNormal(service, cq), appKit_(appKit)
{
    service_->RequestSyncWallet(&ctx_, &request_, &responder_, cq_, cq_, this);
}


void RPCSyncWallet::process()
{
    new RPCSyncWallet(service_, cq_, appKit_);

    joystream::daemon::rpc::Void response;

    appKit_->syncWallet();

    finish(response, true);

}
