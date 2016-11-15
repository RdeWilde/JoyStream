#ifndef SERVERIMPL_HPP
#define SERVERIMPL_HPP

#include <core/Node.hpp>
#include <grpc++/alarm.h>

#include <joystreamd_lib/CompletionQueueDispatcher.hpp>
#include <joystreamd_lib/RPCPause.hpp>
#include <joystreamd_lib/RPCPauseTorrent.hpp>
#include <joystreamd_lib/RPCStartTorrent.hpp>
#include <joystreamd_lib/RPCAddTorrent.hpp>
#include <joystreamd_lib/RPCRemoveTorrent.hpp>
#include <joystreamd_lib/RPCListTorrents.hpp>


class ServerImpl final
{
    public:
        ServerImpl(joystream::core::Node* node, QCoreApplication* app);
        ~ServerImpl();
        void Run();

    private:
        joystream::daemon::rpc::Daemon::AsyncService daemonService_;
        joystream::core::Node *node_;
        QCoreApplication* app_;
        std::unique_ptr<grpc::Server> server_;
        std::unique_ptr<grpc::ServerCompletionQueue> cq_;
        CompletionQueueDispatcher dispatcher_;
        std::thread thread_;
};

#endif // SERVERIMPL_HPP
