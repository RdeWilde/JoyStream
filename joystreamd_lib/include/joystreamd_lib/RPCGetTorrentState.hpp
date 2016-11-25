#ifndef GETTORRENTSTATE_H
#define GETTORRENTSTATE_H

#include <joystreamd_lib/RPCRequestNormal.hpp>
#include <app_kit/kit.hpp>

class RPCGetTorrentState : public RPCRequestNormal<joystream::daemon::rpc::TorrentState, joystream::daemon::rpc::Torrent, joystream::daemon::rpc::Daemon::AsyncService> {
    public:
        RPCGetTorrentState(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void process();

    private:
        joystream::core::Node* node_;
};

#endif // GETTORRENTSTATE_H
