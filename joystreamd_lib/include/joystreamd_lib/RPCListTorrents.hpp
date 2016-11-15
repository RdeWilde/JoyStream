#ifndef RPCLISTTORRENTS_HPP
#define RPCLISTTORRENTS_HPP


#include <joystreamd_lib/RPCRequestStreamServer.hpp>

#include <core/Torrent.hpp>
#include <core/Exception.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Node.hpp>
#include <libtorrent/aux_/escape_string.hpp>

class RPCListTorrents : public RPCRequestStreamServer<joystream::daemon::rpc::Torrent, joystream::daemon::rpc::Void, joystream::daemon::rpc::Daemon::AsyncService> {
    public:
        RPCListTorrents(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void process();

    private:
        joystream::core::Node* node_;
};


#endif // RPCLISTTORRENTS_HPP
