#ifndef RPCUNCONFIRMEDBALANCE_H
#define RPCUNCONFIRMEDBALANCE_H

#include <joystreamd_lib/RPCRequestNormal.hpp>
#include <bitcoin/SPVWallet.hpp>
#include <common/P2PKHAddress.hpp>

class RPCUnconfirmedBalance : public RPCRequestNormal<joystream::daemon::rpc::BTCAmount, joystream::daemon::rpc::Void, joystream::daemon::rpc::Wallet::AsyncService> {
    public:
        RPCUnconfirmedBalance(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet);
        void process();

    private:
        joystream::bitcoin::SPVWallet* wallet_;
};

#endif // RPCUNCONFIRMEDBALANCE_H
