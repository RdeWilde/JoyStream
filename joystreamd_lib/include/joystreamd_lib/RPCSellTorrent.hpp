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
         * @brief sellingModeActivated slot. Will
         * send the final message to the client.
         */
        void sellingModeActivated();

    private:
        joystream::appkit::AppKit* appKit_;
};


#endif // RPCSELLTORRENT_H
