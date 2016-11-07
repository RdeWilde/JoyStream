#include "RPCRemoveTorrent.h"

RPCRemoveTorrent::RPCRemoveTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : service_(service), cq_(cq), node_(node), responder_(&ctx_), RPCRequest()
{
    service_->RequestRemoveTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCRemoveTorrent::onCall()
{
    new RPCRemoveTorrent(service_, cq_, node_);

    libtorrent::sha1_hash info_hash = libtorrent::sha1_hash(request_.infohash());

    std::cout << "Remove Torrent" << std::endl;

    node_->removeTorrent(info_hash, [this](const std::exception_ptr &eptr) {

        joystream::daemon::rpc::TorrentState response;

        if (eptr){
            std::cout << "Caught an error" << std::endl;
            this->responder_.Finish(response, grpc::Status::CANCELLED, this);
        } else {
            std::cout << "Removing Torrent" << std::endl;
            response.set_paused(1);
            this->responder_.Finish(response, grpc::Status::OK, this);
        }
        this->status_ = FINISH;
    });
}
