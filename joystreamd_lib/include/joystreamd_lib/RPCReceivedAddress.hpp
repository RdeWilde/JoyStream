#ifndef RPCRECEIVEDADDRESS_H
#define RPCRECEIVEDADDRESS_H

#include <joystreamd_lib/RPCRequestNormal.hpp>
#include <bitcoin/SPVWallet.hpp>
#include <common/P2PKHAddress.hpp>

class RPCReceivedAddress : public RPCRequestNormal<joystream::daemon::rpc::BitcoinAddress, joystream::daemon::rpc::Void, joystream::daemon::rpc::Wallet::AsyncService> {
    public:
        RPCReceivedAddress(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet);
        void process();

    private:
        joystream::bitcoin::SPVWallet* wallet_;
};

#endif // RPCRECEIVEDADDRESS_H
