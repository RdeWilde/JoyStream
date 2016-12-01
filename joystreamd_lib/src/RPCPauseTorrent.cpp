#include <joystreamd_lib/RPCPauseTorrent.hpp>

RPCPauseTorrent::RPCPauseTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : RPCRequestNormal(service, cq), node_(node)
{
    service_->RequestPauseTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCPauseTorrent::process()
{
    new RPCPauseTorrent(service_, cq_, node_);

    joystream::core::Torrent* torrent;
    joystream::daemon::rpc::Void response;

    torrent = node_->torrent(joystream::appkit::util::sha1_hash_from_hex_string(request_.infohash().c_str()));

    if (torrent != nullptr) {
        // We have the torrent we are going to pause it
        torrent->pause(true, [this, response](const std::exception_ptr &eptr) {

            if (eptr) {
                std::cout << "Something wrong happened when trying to pause torrent" << std::endl;
                this->finish(response, false);
            } else {
                this->finish(response, true);
            }
        });
    } else {
        // Torrent not found in Node return error
        this->finish(response, false);
    }

}
