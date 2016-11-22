#ifndef RPCBUYTORRENT_H
#define RPCBUYTORRENT_H

#include <joystreamd_lib/RPCRequestNormal.hpp>
#include <app_kit/kit.hpp>

class RPCBuyTorrent : public RPCRequestNormal<joystream::daemon::rpc::Void, joystream::daemon::rpc::Torrent, joystream::daemon::rpc::Daemon::AsyncService> {
    public:
        RPCBuyTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::appkit::AppKit* appKit);
        void process();

    private:
        joystream::appkit::AppKit* appKit_;
};

#endif // RPCBUYTORRENT_H
