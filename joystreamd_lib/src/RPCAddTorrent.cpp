#include <joystreamd_lib/RPCAddTorrent.hpp>

RPCAddTorrent::RPCAddTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node, std::string defaultSavePath)
    : RPCRequestNormal(service, cq), node_(node), defaultSavePath_(defaultSavePath)
{
    service_->RequestAddTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCAddTorrent::process()
{
    // Pop up a new instance for concurency
    new RPCAddTorrent(service_, cq_, node_, defaultSavePath_);

    joystream::daemon::rpc::Void response;

    std::string save_path = defaultSavePath_;
    std::vector<char> resume_data = std::vector<char>();
    std::string name;
    boost::optional<uint> upload_limit = -1;
    boost::optional<uint> download_limit = -1;
    bool paused = 0;
    joystream::core::TorrentIdentifier* torrent_identifier;

    switch (request_.type()) {
        case joystream::daemon::rpc::TorrentIdentifierRequest_Type_MAGNETLINK:
            std::cout << "MagnetLink" <<std::endl;
            break;
        case joystream::daemon::rpc::TorrentIdentifierRequest_Type_INFOHASH:
            torrent_identifier = joystream::core::TorrentIdentifier::fromHashString(std::string(request_.infohash()));
            break;
        case joystream::daemon::rpc::TorrentIdentifierRequest_Type_TORRENTFILE:
            std::vector<unsigned char> data(request_.data().begin(), request_.data().end());
            torrent_identifier = joystream::core::TorrentIdentifier::fromTorrentFileContents(data);
            break;
    }

    std::cout << "We are adding the torrent" << std::endl;

    // if torrent_identifier not nullptr it means that hash was correct
    if (torrent_identifier != nullptr) {
        try {
            node_->addTorrent(upload_limit,download_limit,name,resume_data,save_path,paused,torrent_identifier,[this, response](libtorrent::error_code &ecode, libtorrent::torrent_handle &th) {
                if (ecode) {
                    std::cout << "We have an error" << std::endl;
                    this->finish(response, false);
                } else {
                    std::cout << "New torrent added" << std::endl;
                    this->finish(response, true);
                }
            });
        } catch (joystream::core::exception::TorrentAlreadyExists error) {
            std::cout << "Torrent already in Node" << std::endl;
            this->finish(response, false);
        }
    } else {
        this->finish(response, false);
    }

}
