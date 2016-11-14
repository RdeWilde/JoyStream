#ifndef RPCREMOVETORRENT_HPP
#define RPCREMOVETORRENT_HPP

#include "protos/daemon.grpc.pb.h"
#include "protos/daemon.pb.h"

#include <daemon/RPCRequestNormal.hpp>
#include <core/Torrent.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Node.hpp>

class RPCRemoveTorrent : public RPCRequestNormal<joystream::daemon::rpc::TorrentState> {
    public:
        RPCRemoveTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void onCall();

    private:
        joystream::core::Node* node_;
        joystream::daemon::rpc::Torrent request_;

};


#endif // RPCREMOVETORRENT_HPP
