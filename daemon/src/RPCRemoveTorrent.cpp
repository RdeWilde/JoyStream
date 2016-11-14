#include <daemon/RPCRemoveTorrent.hpp>

RPCRemoveTorrent::RPCRemoveTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : RPCRequestNormal(service, cq), node_(node)
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
            this->finish(response, false);
        } else {
            std::cout << "Removing Torrent" << std::endl;
            response.set_paused(1);
            this->finish(response, true);
        }
    });
}
