#ifndef RPCPAUSETORRENT_H
#define RPCPAUSETORRENT_H

#include "RPCRequestNormal.h"
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

#endif // RPCPAUSETORRENT_H
