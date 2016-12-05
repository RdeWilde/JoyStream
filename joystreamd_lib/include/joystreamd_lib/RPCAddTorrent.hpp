#ifndef RPCADDTORRENT_HPP
#define RPCADDTORRENT_HPP


#include <joystreamd_lib/RPCRequestNormal.hpp>

#include <core/Torrent.hpp>
#include <core/Exception.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Node.hpp>

class RPCAddTorrent : public RPCRequestNormal<joystream::daemon::rpc::Void, joystream::daemon::rpc::TorrentIdentifierRequest, joystream::daemon::rpc::Daemon::AsyncService> {
    public:
        RPCAddTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node, std::string defaultSavePath);
        void process();

    private:
        joystream::core::Node* node_;
        std::string defaultSavePath_;
};

#endif // RPCADDTORRENT_HPP
