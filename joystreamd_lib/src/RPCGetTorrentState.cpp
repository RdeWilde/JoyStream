#include <joystreamd_lib/RPCGetTorrentState.hpp>

RPCGetTorrentState::RPCGetTorrentState(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : RPCRequestNormal(service, cq), node_(node)
{
    service_->RequestGetTorrentState(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCGetTorrentState::process()
{
    new RPCGetTorrentState(service_, cq_, node_);

    joystream::core::Torrent* torrent;
    joystream::daemon::rpc::TorrentState response;

    torrent = node_->torrent(joystream::appkit::util::sha1_hash_from_hex_string(request_.infohash().c_str()));

    /*if (torrent != nullptr) {
        std::cout << torrent->state() << std::endl;
        this->finish(response, true);
    } else {
        // Torrent not found in Node return error
        this->finish(response, false);
    }*/

    this->finish(response, true);
}
