#ifndef RPCSUSCRIBEEVENTS_H
#define RPCSUSCRIBEEVENTS_H

#include <joystreamd_lib/RPCRequestStreamServer.hpp>
#include <core/core.hpp>


class RPCSuscribeEvents : public RPCRequestStreamServer<joystream::daemon::rpc::Event, joystream::daemon::rpc::Void, joystream::daemon::rpc::Daemon::AsyncService> {
    public:
        RPCSuscribeEvents(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node);
        void process();

    public slots:
        void publishRemovedTorrent(const libtorrent::sha1_hash & info_hash);
        void publishAddedTorrent(joystream::core::Torrent * torrent);

    private:
        joystream::core::Node* node_;
};

#endif // RPCSUSCRIBEEVENTS_H
