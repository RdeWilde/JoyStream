#include <joystreamd_lib/RPCReceivedAddress.hpp>

RPCReceivedAddress::RPCReceivedAddress(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet)
    : RPCRequestNormal(service, cq), wallet_(wallet)
{
    service_->RequestReceivedAddress(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCReceivedAddress::process()
{
    new RPCReceivedAddress(service_, cq_, wallet_);

    joystream::daemon::rpc::BitcoinAddress response;

    std::string receiveAddress = wallet_->generateReceiveAddress().toBase58CheckEncoding().toStdString();

    response.set_address(receiveAddress);

    finish(response, true);

}
