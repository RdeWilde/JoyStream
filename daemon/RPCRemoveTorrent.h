#ifndef RPCREMOVETORRENT_H
#define RPCREMOVETORRENT_H

#include "protos/daemon.grpc.pb.h"
#include "protos/daemon.pb.h"
#include "RPCRequest.h"
#include <core/Torrent.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Node.hpp>

class RPCRemoveTorrent : public RPCRequest {
    public:
        RPCRemoveTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void onCall();

    private:
        joystream::daemon::rpc::Daemon::AsyncService* service_;
        grpc::ServerCompletionQueue* cq_;
        joystream::core::Node* node_;

        grpc::ServerAsyncResponseWriter<joystream::daemon::rpc::TorrentState> responder_;
        joystream::daemon::rpc::Torrent request_;

};


#endif // RPCREMOVETORRENT_H
