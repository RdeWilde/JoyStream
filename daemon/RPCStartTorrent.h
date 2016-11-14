#ifndef RPCSTARTTORRENT_H
#define RPCSTARTTORRENT_H

#include "RPCRequestNormal.h"
#include <core/Node.hpp>
#include <core/Torrent.hpp>

class RPCStartTorrent : public RPCRequestNormal<joystream::daemon::rpc::Void>{
    public:
        RPCStartTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void onCall();

    private:
        joystream::core::Node* node_;
        joystream::daemon::rpc::Torrent request_;

};

#endif // RPCSTARTTORRENT_H
