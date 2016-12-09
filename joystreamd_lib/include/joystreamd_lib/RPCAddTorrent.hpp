#ifndef RPCADDTORRENT_HPP
#define RPCADDTORRENT_HPP


#include <joystreamd_lib/RPCRequestNormal.hpp>

#include <core/Torrent.hpp>
#include <core/Exception.hpp>
#include <core/TorrentIdentifier.hpp>
#include <app_kit/kit.hpp>

class RPCAddTorrent : public RPCRequestNormal<joystream::daemon::rpc::Void, joystream::daemon::rpc::TorrentIdentifierRequest, joystream::daemon::rpc::Daemon::AsyncService> {
    public:
        RPCAddTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::appkit::AppKit* appKit, std::string defaultSavePath);
        void process();

    public slots:
        /**
         * @brief torrentAdded slot. Will
         * send the final message to the client.
         */
        void torrentAdded();


    private:
        joystream::appkit::AppKit* appKit_;
        std::string defaultSavePath_;
};

#endif // RPCADDTORRENT_HPP
