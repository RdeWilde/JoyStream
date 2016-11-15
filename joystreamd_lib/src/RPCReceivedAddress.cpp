#include <joystreamd_lib/RPCReceivedAddress.hpp>

RPCReceivedAddress::RPCReceivedAddress(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet)
    : RPCRequestNormal(service, cq), wallet_(wallet)
{
    //service_->RequestReceivedAddress(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCReceivedAddress::process()
{
    new RPCReceivedAddress(service_, cq_, wallet_);

    std::cout << "Sending new Bitcoin Address" << std::endl;

    //wallet_->generateReceiveAddress();

    joystream::daemon::rpc::BitcoinAddress response;

    finish(response, true);

}
