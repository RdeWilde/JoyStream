#ifndef RPCADDTORRENTWITHTORRENTFILE_H
#define RPCADDTORRENTWITHTORRENTFILE_H

#include <joystreamd_lib/RPCRequestNormal.hpp>

#include <core/Torrent.hpp>
#include <core/Exception.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Node.hpp>

class RPCAddTorrentWithTorrentFile : public RPCRequestNormal<joystream::daemon::rpc::Void, joystream::daemon::rpc::TorrentFile, joystream::daemon::rpc::Daemon::AsyncService> {
    public:
        RPCAddTorrentWithTorrentFile(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void process();

    private:
        joystream::core::Node* node_;
};

#endif // RPCADDTORRENTWITHTORRENTFILE_H
