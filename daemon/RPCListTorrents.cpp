#include "RPCListTorrents.h"

RPCListTorrents::RPCListTorrents(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : service_(service), cq_(cq), node_(node), responder_(&ctx_), RPCRequest()
{
    service_->RequestListTorrents(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCListTorrents::onCall()
{
    joystream::daemon::rpc::Torrent response;

    // Pop up a new instance for concurency
    new RPCListTorrents(service_, cq_, node_);

    for (const auto t : node_->torrents()) {

        std::cout << libtorrent::to_hex(t.first.to_string()) << std::endl;
        std::cout << "Find one !" << std::endl;
        response.set_infohash(libtorrent::to_hex(t.first.to_string()));
        response.set_name(t.second->name());
        responder_.Write(response, this);
    }

    responder_.Finish(grpc::Status::OK, this);
    status_ = FINISH;
}
