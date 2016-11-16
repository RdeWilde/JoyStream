#include <joystreamd_lib/RPCBalance.hpp>

RPCBalance::RPCBalance(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet)
    : RPCRequestNormal(service, cq), wallet_(wallet)
{
    service_->RequestBalance(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCBalance::process()
{
    new RPCBalance(service_, cq_, wallet_);

    joystream::daemon::rpc::BTCAmount response;

    response.set_amount(wallet_->balance());

    finish(response, true);

}
