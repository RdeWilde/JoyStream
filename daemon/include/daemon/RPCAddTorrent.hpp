#ifndef RPCADDTORRENT_HPP
#define RPCADDTORRENT_HPP

#include "protos/daemon.grpc.pb.h"
#include "protos/daemon.pb.h"

#include <daemon/RPCRequestNormal.hpp>

#include <core/Torrent.hpp>
#include <core/Exception.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Node.hpp>

class RPCAddTorrent : public RPCRequestNormal<joystream::daemon::rpc::Torrent, joystream::daemon::rpc::Torrent> {
    public:
        RPCAddTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void process();

    private:
        joystream::core::Node* node_;
};

#endif // RPCADDTORRENT_HPP
