#include <daemon/RPCStartTorrent.hpp>

RPCStartTorrent::RPCStartTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : RPCRequestNormal(service, cq), node_(node)
{
    service_->RequestStartTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCStartTorrent::process()
{
    new RPCStartTorrent(service_, cq_, node_);

    std::cout << "Trying to start torrent" << std::endl;

    for (const auto t : node_->torrents()) {
        if (libtorrent::to_hex(t.first.to_string()) == request_.infohash()) {

            joystream::daemon::rpc::Void response;

            std::cout << "We are starting the torrent" << std::endl;

            if (t.second->isPaused()){
                t.second->resume([this, response](const std::exception_ptr &eptr) {
                    if (eptr) {
                        std::cout << "Something wrong happened when trying to resume the torrent" << std::endl;
                        this->finish(response, false);
                    } else {
                        this->finish(response, true);
                    }
                });
            } else {
                this->finish(response, true);
            }

            break;
        }
    }

}
