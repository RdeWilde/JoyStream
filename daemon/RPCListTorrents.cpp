#include "RPCListTorrents.h"

RPCListTorrents::RPCListTorrents(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : service_(service), cq_(cq), node_(node), responder_(&ctx_), RPCRequest()
{
    service_->RequestListTorrents(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCListTorrents::onCall()
{
    // Pop up a new instance for concurency
    new RPCListTorrents(service_, cq_, node_);

    for (const auto t : node_->torrents()) {
      std::cout << t.second->infoHash() << std::endl;
      std::cout << "Find one !" << std::endl;
      //response_.set_infohash(t.second->infoHash().to_string());
      response_.set_name(t.second->name());
      responder_.Write(response_, this);
    }

    responder_.Finish(grpc::Status::OK, this);
    status_ = FINISH;
}
