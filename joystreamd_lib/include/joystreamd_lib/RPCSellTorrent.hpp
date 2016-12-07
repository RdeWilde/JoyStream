#ifndef RPCSELLTORRENT_H
#define RPCSELLTORRENT_H

#include <joystreamd_lib/RPCRequestNormal.hpp>
#include <app_kit/kit.hpp>
#include <QObject>


class RPCSellTorrent : public RPCRequestNormal<joystream::daemon::rpc::Void, joystream::daemon::rpc::SellTorrentRequest, joystream::daemon::rpc::Daemon::AsyncService> {
    public:
        RPCSellTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::appkit::AppKit* appKit);
        void process();

    public slots:
        /**
         * @brief Verify if torrent status is `seeding` so
         * it can start to sell torrent.
         * */
        void checkStatus(libtorrent::torrent_status::state_t state, float progress);
        /**
         * @brief Get called once a plugin is added to
         * the torrent
         * */
        void pluginAdded(joystream::core::TorrentPlugin* torrentPlugin);

    private:
        joystream::appkit::AppKit* appKit_;
};


#endif // RPCSELLTORRENT_H
