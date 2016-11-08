#include "RPCPauseTorrent.h"

RPCPauseTorrent::RPCPauseTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : RPCRequest(), service_(service), cq_(cq), node_(node), responder_(&ctx_)
{
    service_->RequestPauseTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCPauseTorrent::onCall()
{
    new RPCPauseTorrent(service_, cq_, node_);

    std::cout << "Trying to pause torrent" << std::endl;

    // A getTorrent function in Node class would be better
    for (const auto t : node_->torrents()) {
        if (libtorrent::to_hex(t.first.to_string()) == request_.infohash()) {
            std::cout << "We are pausing the torrent" << std::endl;
            t.second->pause(true, [this](const std::exception_ptr &eptr) {
                joystream::daemon::rpc::Void response;

                if (eptr) {
                    std::cout << "Something wrong happened when trying to pause torrent" << std::endl;
                    this->responder_.Finish(response, grpc::Status::CANCELLED, this);
                } else {
                    this->responder_.Finish(response, grpc::Status::OK, this);
                }
                this->status_ = FINISH;
            });

            break;
        }
    }

}
