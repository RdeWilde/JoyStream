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
using joystream::daemon::rpc::Hello;
using joystream::daemon::rpc::Void;
using joystream::daemon::rpc::Answer;

// Explicit template instantiation of IdToString()
// used in joystream::protocol_session::exception::ConnectionAlreadyAddedException
template <>
std::string IdToString<boost::asio::ip::basic_endpoint<boost::asio::ip::tcp>>(boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> const&id){
    return id.address().to_string();
}

class SyncService : public Daemon::Service {
  public:
    SyncService(joystream::core::Node* node, QCoreApplication* app, Hello::AsyncService* service, ServerCompletionQueue* cq)
        : node_(node),
          app_(app),
          service_(service),
          cq_(cq)
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
        std::cout << std::get<0>(e) << std::endl;
        writer->Write(torrent);
      }

      service_->RequestSayHello(&ctx_, &request_, &reply_, cq_, cq_,this);
      reply_.set_what("What");


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
       bool done = 0;
       std::cout << "We are adding the torrent" << std::endl;
       std::cout << request->infohash() << std::endl;
       node_->addTorrent(upload_limit,download_limit,name,resume_data,save_path,paused,torrent_identifier,[&done](libtorrent::error_code &ecode, libtorrent::torrent_handle &th) {
           std::cout << "New torrent added" << std::endl;
           done = 1;
       });
       // Waiting for the callback to be done
       // Should be async
       while(true){
          if (done){
           return Status::OK;
          }
       }
     }

  private:
    joystream::core::Node *node_;
    QCoreApplication *app_;
    std::unique_ptr<ServerCompletionQueue> cq_;
    Hello::AsyncService service_;
    ServerContext ctx_;

    // What we get from the client.
    Void request_;
    // What we send back to the client.
    Answer reply_;

    // The means to get back to the client.
    ServerAsyncResponseWriter<Answer> responder_;
};

class DaemonServiceImpl final {
  public:

    DaemonServiceImpl(joystream::core::Node* node, QCoreApplication* app)
        : node_(node),
          app_(app)
    {}

    ~DaemonServiceImpl() {
      server_->Shutdown();
      // Always shutdown the completion queue after the server.
      cq_->Shutdown();
    }

    // There is no shutdown handling in this code.
    void Run() {
      std::string server_address("0.0.0.0:3002");

      ServerBuilder builder;
      SyncService syncService_(node_,app_);
      // Listen on the given address without any authentication mechanism.
      builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
      // Register "service_" as the instance through which we'll communicate with
      // clients. In this case it corresponds to an *asynchronous* service.
      builder.RegisterService(&service_);
      builder.RegisterService(&syncService_);
      // Get hold of the completion queue used for the asynchronous communication
      // with the gRPC runtime.
      cq_ = builder.AddCompletionQueue();
      // Finally assemble the server.
      server_ = builder.BuildAndStart();
      std::cout << "Server listening on " << server_address << std::endl;

      // Proceed to the server's main loop.
      HandleRpcs();
    }

  private:

    class CallData {
      public:
      // Take in the "service" instance (in this case representing an asynchronous
      // server) and the completion queue "cq" used for asynchronous communication
      // with the gRPC runtime.
      CallData(Hello::AsyncService* service, ServerCompletionQueue* cq)
        : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
      // Invoke the serving logic right away.
      Proceed();
      //std::cout << "Hey there" << std::endl;
    }

    void Proceed() {
      if (status_ == CREATE) {
        // Make this instance progress to the PROCESS state.
        status_ = PROCESS;

        std::cout << "Something happening here ?" << std::endl;

        // As part of the initial CREATE state, we *request* that the system
        // start processing SayHello requests. In this request, "this" acts are
        // the tag uniquely identifying the request (so that different CallData
        // instances can serve different requests concurrently), in this case
        // the memory address of this CallData instance.
        service_->RequestSayHello(&ctx_, &request_, &responder_, cq_, cq_,this);

      } else if (status_ == PROCESS) {
        // Spawn a new CallData instance to serve new clients while we process
        // the one for this CallData. The instance will deallocate itself as
        // part of its FINISH state.
        new CallData(service_, cq_);

        std::cout << "We are processing the request !" << std::endl;

        // The actual processing.
        reply_.set_what("What");

        // And we are done! Let the gRPC runtime know we've finished, using the
        // memory address of this instance as the uniquely identifying tag for
        // the event.
        status_ = FINISH;
        responder_.Finish(reply_, Status::OK, this);
      } else {
        GPR_ASSERT(status_ == FINISH);
        // Once in the FINISH state, deallocate ourselves (CallData).
        delete this;
      }
    }

     private:
       // The means of communication with the gRPC runtime for an asynchronous
       // server.
       Hello::AsyncService* service_;

       // The producer-consumer queue where for asynchronous server notifications.
       ServerCompletionQueue* cq_;
       // Context for the rpc, allowing to tweak aspects of it such as the use
       // of compression, authentication, as well as to send metadata back to the
       // client.
       ServerContext ctx_;

       // What we get from the client.
       Void request_;
       // What we send back to the client.
       Answer reply_;

       // The means to get back to the client.
       ServerAsyncResponseWriter<Answer> responder_;

       // Let's implement a tiny state machine with the following states.
       enum CallStatus { CREATE, PROCESS, FINISH };
       CallStatus status_;  // The current serving state.
    };
    // This can be run in multiple threads if needed.
    void HandleRpcs() {
      // Spawn a new CallData instance to serve new clients.
      new CallData(&service_, cq_.get());
      void* tag;  // uniquely identifies a request.
      bool ok;
      while (true) {
        // Block waiting to read the next event from the completion queue. The
        // event is uniquely identified by its tag, which in this case is the
        // memory address of a CallData instance.
        // The return value of Next should always be checked. This return value
        // tells us whether there is any kind of event or cq_ is shutting down.
        GPR_ASSERT(cq_->Next(&tag, &ok));
        GPR_ASSERT(ok);
        static_cast<CallData*>(tag)->Proceed();
      }
   }

    joystream::core::Node *node_;
    QCoreApplication *app_;
    std::unique_ptr<ServerCompletionQueue> cq_;
    Hello::AsyncService service_;
    std::unique_ptr<Server> server_;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create the objects we need here to pass to the daemon
    auto node = joystream::core::Node::create([](const Coin::Transaction &tx){
        // broadcast tx
    });

    DaemonServiceImpl service(node, &a);

    service.Run();

    // We need a Qt event loop
    a.exec();
}
