#include <QCoreApplication>
#include <core/Node.hpp>

#include <boost/asio/impl/src.hpp>

#include <thread>
#include <grpc++/grpc++.h>

#include "protos/daemon.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using joystream::daemon::rpc::Daemon;
using joystream::daemon::rpc::Void;

// Explicit template instantiation of IdToString()
// used in joystream::protocol_session::exception::ConnectionAlreadyAddedException
template <>
std::string IdToString<boost::asio::ip::basic_endpoint<boost::asio::ip::tcp>>(boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> const&id){
    return id.address().to_string();
}



class DaemonServiceImpl final : public Daemon::Service {
public:
  DaemonServiceImpl(joystream::core::Node* node) {
      node = node_;
  }

  Status Pause(ServerContext* context, const Void* request, Void* reply) override {
      std::cout << "Received Pause Request" << std::endl;

      node_->pause([](){
          std::cout << "Node was paused" << std::endl;
      });

      return Status::OK;
  }

  std::unique_ptr<Server> CreateServer() {
      std::string server_address("0.0.0.0:30000");

      ServerBuilder builder;
      builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
      builder.RegisterService(this);
      std::unique_ptr<Server> server(builder.BuildAndStart());
      std::cout << "RPC Server listening on " << server_address << std::endl;
      return server;
  }

private:
  joystream::core::Node *node_;
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create the objects we need here to pass to the daemon
    joystream::core::Node node([](const Coin::Transaction &tx){
        // broadcast tx
    });

    // Create a daemon rpc service
    DaemonServiceImpl service(&node);

    // Build and Start the RPC service
    std::unique_ptr<Server> server = service.CreateServer();

    // Process RPC requests on a new thread
    std::thread rpc([&](){
        server->Wait();
    });

    // We need a Qt event loop
    return a.exec();

    rpc.join();
}
