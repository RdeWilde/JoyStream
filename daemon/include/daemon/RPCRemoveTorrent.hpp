#ifndef RPCREMOVETORRENT_HPP
#define RPCREMOVETORRENT_HPP

#include <daemon/RPCRequestNormal.hpp>
#include <core/Torrent.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Node.hpp>

class RPCRemoveTorrent : public RPCRequestNormal<joystream::daemon::rpc::TorrentState, joystream::daemon::rpc::Torrent> {
    public:
        RPCRemoveTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void process();

    private:
        joystream::core::Node* node_;

};


#endif // RPCREMOVETORRENT_HPP
