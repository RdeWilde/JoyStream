#include <QCoreApplication>
#include <core/Node.hpp>
#include <core/TorrentIdentifier.hpp>

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
  DaemonServiceImpl(joystream::core::Node* node, QCoreApplication* app)
      : node_(node),
        app_(app)
  {}

  Status Pause(ServerContext* context, const Void* request, Void* reply) override {
      std::cout << "Received Pause Request" << std::endl;

      node_->pause([this](){
          std::cout << "Node was paused" << std::endl;
          this->app_->exit();
      });

      return Status::OK;
  }

  Status ListTorrents(grpc::ServerContext *context, const joystream::daemon::rpc::Void *request, ::grpc::ServerWriter<joystream::daemon::rpc::Torrent> *writer) override {
    joystream::daemon::rpc::Torrent torrent;
    std::cout << "Torrents list" << std::endl;
    for (const auto &e : node_->torrents()) {
      writer->Write(torrent);
    }
    return Status::OK;
  }

  Status AddTorrent(grpc::ServerContext *context, const joystream::daemon::rpc::Torrent* request, joystream::daemon::rpc::Torrent* torrent) override {
     libtorrent::sha1_hash info_hash = libtorrent::sha1_hash(request->infohash());
     std::string save_path = std::string("/home/lola/joystream");
     std::vector<char> resume_data = std::vector<char>();
     std::string name = std::string(request->name());
     boost::optional<uint> upload_limit = -1;
     boost::optional<uint> download_limit = -1;
     bool paused = 1;
     joystream::core::TorrentIdentifier torrent_identifier = joystream::core::TorrentIdentifier(info_hash);
     joystream::core::Node::AddedTorrent added_torrent;
     std::cout << "We are adding the torrent" << std::endl;
     std::cout << request->infohash() << std::endl;
     node_->addTorrent(upload_limit,download_limit,name,resume_data,save_path,paused,torrent_identifier,added_torrent);
     std::cout << "New torrent added" << std::endl;
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
  QCoreApplication *app_;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create the objects we need here to pass to the daemon
    auto node = joystream::core::Node::create([](const Coin::Transaction &tx){
        // broadcast tx
    });

    // Create a daemon rpc service
    DaemonServiceImpl service(node, &a);

    // Build and Start the RPC service
    std::unique_ptr<Server> server = service.CreateServer();

    // We need a Qt event loop
    a.exec();

    server->Shutdown();
}
