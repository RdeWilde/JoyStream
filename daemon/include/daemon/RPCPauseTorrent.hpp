#ifndef RPCPAUSETORRENT_HPP
#define RPCPAUSETORRENT_HPP

#include "protos/daemon.grpc.pb.h"
#include "protos/daemon.pb.h"

#include <daemon/RPCRequestNormal.hpp>
#include <core/Node.hpp>
#include <core/Torrent.hpp>

class RPCPauseTorrent : public RPCRequestNormal<joystream::daemon::rpc::Void> {
    public:
        RPCPauseTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void onCall();

    private:
        joystream::core::Node* node_;
        joystream::daemon::rpc::Torrent request_;
};

#endif // RPCPAUSETORRENT_HPP
