#ifndef SERVERIMPL_HPP
#define SERVERIMPL_HPP

#include <core/Node.hpp>
#include <bitcoin/SPVWallet.hpp>
#include <grpc++/alarm.h>
#include <app_kit/kit.hpp>

#include <joystreamd_lib/CompletionQueueDispatcher.hpp>
#include <joystreamd_lib/RPCPause.hpp>
#include <joystreamd_lib/RPCPauseTorrent.hpp>
#include <joystreamd_lib/RPCStartTorrent.hpp>
#include <joystreamd_lib/RPCAddTorrent.hpp>
#include <joystreamd_lib/RPCRemoveTorrent.hpp>
#include <joystreamd_lib/RPCListTorrents.hpp>
#include <joystreamd_lib/RPCReceivedAddress.hpp>
#include <joystreamd_lib/RPCBalance.hpp>
#include <joystreamd_lib/RPCUnconfirmedBalance.hpp>
#include <joystreamd_lib/RPCStatus.hpp>


class ServerImpl final
{
    public:
        ServerImpl(joystream::appkit::AppKit* kit, QCoreApplication* app);
        void Shutdown();
        void Run();

    private:
        joystream::daemon::rpc::Daemon::AsyncService daemonService_;
        joystream::daemon::rpc::Wallet::AsyncService walletService_;
        joystream::appkit::AppKit* kit_;
        QCoreApplication* app_;
        std::unique_ptr<grpc::Server> server_;
        std::unique_ptr<grpc::ServerCompletionQueue> cq_;
        CompletionQueueDispatcher dispatcher_;
        std::thread thread_;
};

#endif // SERVERIMPL_HPP
