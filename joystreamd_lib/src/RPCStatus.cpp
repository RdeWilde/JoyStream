#include <joystreamd_lib/RPCStatus.hpp>

RPCStatus::RPCStatus(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet)
    : RPCRequestNormal(service, cq), wallet_(wallet)
{
    service_->RequestStatus(&ctx_, &request_, &responder_, cq_, cq_, this);
}

joystream::daemon::rpc::WalletStatus_Status RPCStatus::getStatus(const joystream::bitcoin::SPVWallet::wallet_status_t status)
{
    switch(status) {
        case joystream::bitcoin::SPVWallet::wallet_status_t::OFFLINE:
            return joystream::daemon::rpc::WalletStatus_Status_OFFLINE;
        case joystream::bitcoin::SPVWallet::wallet_status_t::CONNECTING:
            return joystream::daemon::rpc::WalletStatus_Status_CONNECTING;
        case joystream::bitcoin::SPVWallet::wallet_status_t::SYNCHING_HEADERS:
            return joystream::daemon::rpc::WalletStatus_Status_SYNCHING_HEADERS;
        case joystream::bitcoin::SPVWallet::wallet_status_t::SYNCHING_BLOCKS:
            return joystream::daemon::rpc::WalletStatus_Status_SYNCHING_BLOCKS;
        case joystream::bitcoin::SPVWallet::wallet_status_t::SYNCHED:
            return joystream::daemon::rpc::WalletStatus_Status_SYNCHED;
    };

}

void RPCStatus::process()
{
    new RPCStatus(service_, cq_, wallet_);

    joystream::daemon::rpc::WalletStatus response;

    response.set_status(getStatus(wallet_->status()));

    finish(response, true);

}
