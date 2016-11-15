#ifndef RPCPAUSETORRENT_HPP
#define RPCPAUSETORRENT_HPP

#include <daemon/RPCRequestNormal.hpp>
#include <core/Node.hpp>
#include <core/Torrent.hpp>

class RPCPauseTorrent : public RPCRequestNormal<joystream::daemon::rpc::Void, joystream::daemon::rpc::Torrent> {
    public:
        RPCPauseTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void process();

    private:
        joystream::core::Node* node_;
};

#endif // RPCPAUSETORRENT_HPP
