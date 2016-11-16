#ifndef RPCBALANCE_H
#define RPCBALANCE_H

#include <joystreamd_lib/RPCRequestNormal.hpp>
#include <bitcoin/SPVWallet.hpp>
#include <common/P2PKHAddress.hpp>

class RPCBalance : public RPCRequestNormal<joystream::daemon::rpc::BTCAmount, joystream::daemon::rpc::Void, joystream::daemon::rpc::Wallet::AsyncService> {
    public:
        RPCBalance(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet);
        void process();

    private:
        joystream::bitcoin::SPVWallet* wallet_;
};

#endif // RPCBALANCE_H
