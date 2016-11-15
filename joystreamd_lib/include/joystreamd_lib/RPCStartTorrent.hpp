#ifndef RPCSTARTTORRENT_HPP
#define RPCSTARTTORRENT_HPP

#include <joystreamd_lib/RPCRequestNormal.hpp>
#include <core/Node.hpp>
#include <core/Torrent.hpp>

class RPCStartTorrent : public RPCRequestNormal<joystream::daemon::rpc::Void, joystream::daemon::rpc::Torrent>{
    public:
        RPCStartTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void process();

    private:
        joystream::core::Node* node_;
};

#endif // RPCSTARTTORRENT_HPP
