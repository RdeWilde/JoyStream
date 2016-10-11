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
using grpc::ServerCompletionQueue;
using grpc::ServerAsyncResponseWriter;
using grpc::Status;
using joystream::daemon::rpc::Daemon;
using joystream::daemon::rpc::Callback;
using joystream::daemon::rpc::Void;
using joystream::daemon::rpc::Torrent;

// Explicit template instantiation of IdToString()
// used in joystream::protocol_session::exception::ConnectionAlreadyAddedException
template <>
std::string IdToString<boost::asio::ip::basic_endpoint<boost::asio::ip::tcp>>(boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> const&id){
    return id.address().to_string();
}

class CallData {
 public:
  CallData(Callback::AsyncService* service, ServerCompletionQueue* cq)
      : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
    // Invoke the serving logic right away.
    Proceed();
  }

  void Proceed() {
    if (status_ == CREATE) {

      service_->RequestTorrentAdded(&ctx_, &request_, &responder_, cq_, cq_,this);
      status_ = PROCESS;

    } else if (status_ == PROCESS) {

      new CallData(service_, cq_);

    } else {

      GPR_ASSERT(status_ == FINISH);
      std::cout << "Destroy" << std::endl;
      delete this;

    }
  }

  void Announce(Torrent torrent) {
    if(status_ != PROCESS){
      return;
    }

    status_ = FINISH;

    responder_.Finish(torrent, Status::OK, this);
  }

 private:

  Callback::AsyncService* service_;
  ServerCompletionQueue* cq_;
  ServerContext ctx_;

  Void request_;
  Torrent reply_;
  ServerAsyncResponseWriter<Torrent> responder_;

  enum CallStatus { CREATE, PROCESS, FINISH };
  CallStatus status_;  // The current serving state.
};

class DaemonServiceImpl : public Daemon::Service {

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
        std::cout << std::get<0>(e) << std::endl;
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
       std::cout << "We are adding the torrent" << std::endl;
       std::cout << request->infohash() << std::endl;
       node_->addTorrent(upload_limit,download_limit,name,resume_data,save_path,paused,torrent_identifier,[&name, &info_hash, this](libtorrent::error_code &ecode, libtorrent::torrent_handle &th) {

           Torrent reply_;

           std::cout << "New torrent added" << std::endl;
           std::cout << ecode << std::endl;
           reply_.set_name(name);
           reply_.set_infohash(info_hash.to_string());
           if(this->tag_)
             static_cast<CallData*>(tag_)->Announce(reply_);
       });
       return Status::OK;
     }


  public:
    DaemonServiceImpl(joystream::core::Node* node, QCoreApplication* app)
        : node_(node), app_(app)
    {}

    void setTag(void* t) {
        std::cout << "setTag : " << t << std::endl;
        tag_ = t;
    }

  private:
    joystream::core::Node *node_;
    QCoreApplication *app_;
    void* tag_;
};

class ServerImpl final {
  public:

    ServerImpl(joystream::core::Node* node, QCoreApplication* app)
        : node_(node),
          app_(app)
    {}

    ~ServerImpl() {
      server_->Shutdown();
      // Always shutdown the completion queue after the server.
      cq_->Shutdown();
    }

    // There is no shutdown handling in this code.
    void Run() {
      std::string server_address("0.0.0.0:3002");

      ServerBuilder builder;
      DaemonServiceImpl syncService_(node_,app_);

      builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

      builder.RegisterService(&syncService_); // sync service
      builder.RegisterService(&asyncService_); // async service

      cq_ = builder.AddCompletionQueue();
      server_ = builder.BuildAndStart();
      std::cout << "Server listening on " << server_address << std::endl;

      HandleRpcs(&syncService_);
    }

  private:
    // This can be run in multiple threads if needed.
    void HandleRpcs(DaemonServiceImpl *syncService) {
      new CallData(&asyncService_, cq_.get());
      void* tag;
      bool ok;
      while (true) {
        GPR_ASSERT(cq_->Next(&tag, &ok));
        GPR_ASSERT(ok);
        syncService->setTag(tag);
        static_cast<CallData*>(tag)->Proceed();
      }
    }

    joystream::core::Node *node_;
    QCoreApplication *app_;
    std::unique_ptr<ServerCompletionQueue> cq_;
    std::unique_ptr<Server> server_;
    Callback::AsyncService asyncService_;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create the objects we need here to pass to the daemon
    auto node = joystream::core::Node::create([](const Coin::Transaction &tx){
        // broadcast tx
    });

    ServerImpl server(node, &a);

    server.Run();

    return 0;
}
