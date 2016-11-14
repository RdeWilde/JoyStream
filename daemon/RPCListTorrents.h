#ifndef RPCLISTTORRENTS_H
#define RPCLISTTORRENTS_H


#include "protos/daemon.grpc.pb.h"
#include "protos/daemon.pb.h"
#include "RPCRequestStreamServer.h"

#include <core/Torrent.hpp>
#include <core/Exception.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Node.hpp>
#include <libtorrent/aux_/escape_string.hpp>

class RPCListTorrents : public RPCRequestStreamServer<joystream::daemon::rpc::Torrent> {
    public:
        RPCListTorrents(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void onCall();

    private:
        joystream::core::Node* node_;
        joystream::daemon::rpc::Void request_;

};


#endif // RPCLISTTORRENTS_H
