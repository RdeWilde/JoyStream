#ifndef RPCBUYTORRENT_H
#define RPCBUYTORRENT_H

#include <joystreamd_lib/RPCRequestNormal.hpp>
#include <app_kit/kit.hpp>
#include <QObject>


class RPCBuyTorrent : public RPCRequestNormal<joystream::daemon::rpc::Void, joystream::daemon::rpc::Torrent, joystream::daemon::rpc::Daemon::AsyncService> {
    public:
        RPCBuyTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::appkit::AppKit* appKit);
        void process();

    public slots:
        /**
         * @brief Verify if torrent status is `downloading` so
         * it can start buying torrent.
         * */
        void checkStatus(libtorrent::torrent_status::state_t state, float progress);

    private:
        joystream::appkit::AppKit* appKit_;
};

#endif // RPCBUYTORRENT_H
