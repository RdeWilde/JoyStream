#include <joystreamd_lib/RPCGetTorrentState.hpp>

RPCGetTorrentState::RPCGetTorrentState(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : RPCRequestNormal(service, cq), node_(node)
{
    service_->RequestGetTorrentState(&ctx_, &request_, &responder_, cq_, cq_, this);
}

joystream::daemon::rpc::TorrentState_State RPCGetTorrentState::getState(const libtorrent::torrent_status::state_t state)
{
    switch(state) {
        // cannot find it but it is in the lib ?
        //case libtorrent::torrent_status::state_t::queued_for_checking:
        //    return joystream::daemon::rpc::TorrentState_State_QUEUED_FOR_CHECKING;
        case libtorrent::torrent_status::state_t::checking_files:
            return joystream::daemon::rpc::TorrentState_State_CHECKING_FILES;
        case libtorrent::torrent_status::state_t::downloading:
            return joystream::daemon::rpc::TorrentState_State_DOWNLOADING;
        case libtorrent::torrent_status::state_t::finished:
            return joystream::daemon::rpc::TorrentState_State_FINISHED;
        case libtorrent::torrent_status::state_t::seeding:
            return joystream::daemon::rpc::TorrentState_State_SEEDING;
        case libtorrent::torrent_status::state_t::allocating:
            return joystream::daemon::rpc::TorrentState_State_ALLOCATING;
    };

}

void RPCGetTorrentState::process()
{
    new RPCGetTorrentState(service_, cq_, node_);

    joystream::core::Torrent* torrent;
    joystream::daemon::rpc::TorrentState response;

    torrent = node_->torrent(joystream::appkit::util::sha1_hash_from_hex_string(request_.infohash().c_str()));

    if (torrent != nullptr) {
        //response.set_state(getState(torrent->state()));
        // torrent->state() throw sometimes a libtorrent error.
        //std::cout << torrent->state() << std::endl;
        response.set_state(joystream::daemon::rpc::TorrentState_State_DOWNLOADING);
        std::cout << "Find Torrent" << std::endl;
        this->finish(response, true);
    } else {
        // Torrent not found in Node return error
        this->finish(response, false);
    }
}
