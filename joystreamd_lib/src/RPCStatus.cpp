#include <joystreamd_lib/RPCStatus.hpp>

RPCStatus::RPCStatus(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet)
    : RPCRequestNormal(service, cq), wallet_(wallet)
{
    service_->RequestStatus(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCStatus::process()
{
    new RPCStatus(service_, cq_, wallet_);

    joystream::daemon::rpc::WalletStatus response;

    /*
    enum class wallet_status_t
    {
        UNINITIALIZED,      // wallet database file not yet opened, netsync stopped
                            // this is the starting state of the wallet, it will never go back
                            // to this state after successful opening/creating of the wallet database

        OFFLINE,            // wallet database ready, netsync stopped

        CONNECTING,         // netsync connecting, starting background threads

        SYNCHING_HEADERS,   // netsync is retrieving headers and updating blocktree

        SYNCHING_BLOCKS,    // netsync is retreiving filtered blocks and the wallet is
                            // updating the store block headers and transactions

        SYNCHED             // receiving mempool (unconfirmed transactions) and waiting for next block
    };*/

    switch(wallet_->status()) {
        case joystream::bitcoin::SPVWallet::wallet_status_t::UNINITIALIZED:
            response.set_status(joystream::daemon::rpc::WalletStatus_Status_UNINITIALIZED);
        case joystream::bitcoin::SPVWallet::wallet_status_t::OFFLINE:
            response.set_status(joystream::daemon::rpc::WalletStatus_Status_OFFLINE);
        case joystream::bitcoin::SPVWallet::wallet_status_t::CONNECTING:
            response.set_status(joystream::daemon::rpc::WalletStatus_Status_CONNECTING);
        case joystream::bitcoin::SPVWallet::wallet_status_t::SYNCHING_HEADERS:
            response.set_status(joystream::daemon::rpc::WalletStatus_Status_SYNCHING_HEADERS);
        case joystream::bitcoin::SPVWallet::wallet_status_t::SYNCHING_BLOCKS:
            response.set_status(joystream::daemon::rpc::WalletStatus_Status_SYNCHING_BLOCKS);
        case joystream::bitcoin::SPVWallet::wallet_status_t::SYNCHED:
            response.set_status(joystream::daemon::rpc::WalletStatus_Status_SYNCHED);
    };

    finish(response, true);

}
