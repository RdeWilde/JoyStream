#ifndef RPCSYNCWALLET_H
#define RPCSYNCWALLET_H

#include <joystreamd_lib/RPCRequestNormal.hpp>
#include <app_kit/AppKit.hpp>

class RPCSyncWallet : public RPCRequestNormal<joystream::daemon::rpc::Void, joystream::daemon::rpc::Void, joystream::daemon::rpc::Wallet::AsyncService> {
    public:
        RPCSyncWallet(joystream::daemon::rpc::Wallet::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::appkit::AppKit* appKit);
        void process();

    private:
        joystream::appkit::AppKit* appKit_;
};


#endif // RPCSYNCWALLET_H
