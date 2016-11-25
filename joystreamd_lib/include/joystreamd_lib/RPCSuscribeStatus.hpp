#ifndef RPCSUSCRIBESTATUS_H
#define RPCSUSCRIBESTATUS_H

#include <joystreamd_lib/RPCRequestStreamServer.hpp>
#include <bitcoin/SPVWallet.hpp>
#include <QMetaType>


class RPCSuscribeStatus : public RPCRequestStreamServer<joystream::daemon::rpc::WalletStatus, joystream::daemon::rpc::Void, joystream::daemon::rpc::Wallet::AsyncService> {
    public:
        RPCSuscribeStatus(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::bitcoin::SPVWallet* wallet);
        void process();

    public slots:
        void publishToClient(const joystream::bitcoin::SPVWallet::wallet_status_t status);

    private:
        joystream::bitcoin::SPVWallet* wallet_;
};

#endif // RPCSUSCRIBESTATUS_H
