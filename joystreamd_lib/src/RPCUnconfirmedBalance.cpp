#include <joystreamd_lib/RPCUnconfirmedBalance.hpp>

RPCUnconfirmedBalance::RPCUnconfirmedBalance(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet)
    : RPCRequestNormal(service, cq), wallet_(wallet)
{
    service_->RequestUnconfirmedBalance(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCUnconfirmedBalance::process()
{
    new RPCUnconfirmedBalance(service_, cq_, wallet_);

    joystream::daemon::rpc::BTCAmount response;

    response.set_amount(wallet_->unconfirmedBalance());

    finish(response, true);

}
