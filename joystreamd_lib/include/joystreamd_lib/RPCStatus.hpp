#ifndef RPCSTATUS_H
#define RPCSTATUS_H

#include <joystreamd_lib/RPCRequestNormal.hpp>
#include <bitcoin/SPVWallet.hpp>

class RPCStatus : public RPCRequestNormal<joystream::daemon::rpc::WalletStatus, joystream::daemon::rpc::Void, joystream::daemon::rpc::Wallet::AsyncService> {
    public:
        RPCStatus(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet);
        void process();

    private:
        joystream::bitcoin::SPVWallet* wallet_;
};


#endif // RPCSTATUS_H
