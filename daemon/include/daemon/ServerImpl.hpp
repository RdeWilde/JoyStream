#ifndef SERVERIMPL_HPP
#define SERVERIMPL_HPP

#include <core/Node.hpp>
#include <grpc++/alarm.h>

#include <daemon/CompletionQueueDispatcher.hpp>
#include <daemon/RPCPause.hpp>
#include <daemon/RPCPauseTorrent.hpp>
#include <daemon/RPCStartTorrent.hpp>
#include <daemon/RPCAddTorrent.hpp>
#include <daemon/RPCRemoveTorrent.hpp>
#include <daemon/RPCListTorrents.hpp>


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
