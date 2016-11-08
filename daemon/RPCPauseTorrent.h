#ifndef RPCPAUSETORRENT_H
#define RPCPAUSETORRENT_H

#include "RPCRequest.h"
#include <core/Node.hpp>
#include <core/Torrent.hpp>

class RPCPauseTorrent : public RPCRequest {
public:
    RPCPauseTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
    void onCall();

private:
    joystream::daemon::rpc::Daemon::AsyncService* service_;
    grpc::ServerCompletionQueue* cq_;
    joystream::core::Node* node_;

    grpc::ServerAsyncResponseWriter<joystream::daemon::rpc::Void> responder_;
    joystream::daemon::rpc::Torrent request_;
    grpc::ServerContext ctx_;

};

#endif // RPCPAUSETORRENT_H
