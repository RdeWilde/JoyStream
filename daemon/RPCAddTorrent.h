#ifndef RPCADDTORRENT_H
#define RPCADDTORRENT_H

#include "protos/daemon.grpc.pb.h"
#include "protos/daemon.pb.h"
#include "RPCRequestNormal.h"

#include <core/Torrent.hpp>
#include <core/Exception.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Node.hpp>

class RPCAddTorrent : public RPCRequestNormal<joystream::daemon::rpc::Torrent> {
    public:
        RPCAddTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void onCall();

    private:
        joystream::core::Node* node_;

        joystream::daemon::rpc::Torrent request_;

};

#endif // RPCADDTORRENT_H
