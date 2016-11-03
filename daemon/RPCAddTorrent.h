#ifndef RPCADDTORRENT_H
#define RPCADDTORRENT_H

#include "protos/daemon.grpc.pb.h"
#include "protos/daemon.pb.h"
#include "RPCRequest.h"
#include <core/Torrent.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Node.hpp>

class RPCAddTorrent : public RPCRequest {
    public:
        RPCAddTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void onCall();

    private:
        joystream::daemon::rpc::Daemon::AsyncService* service_;
        grpc::ServerCompletionQueue* cq_;
        joystream::core::Node* node_;

        joystream::daemon::rpc::Torrent response_;
        grpc::ServerAsyncResponseWriter<joystream::daemon::rpc::Torrent> responder_;
        joystream::daemon::rpc::Torrent request_;
        grpc::ServerContext ctx_;

};

#endif // RPCADDTORRENT_H
