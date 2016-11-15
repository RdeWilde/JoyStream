#include <daemon/RPCAddTorrent.hpp>

RPCAddTorrent::RPCAddTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : RPCRequestNormal(service, cq), node_(node)
{
    service_->RequestAddTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCAddTorrent::process()
{
    // Pop up a new instance for concurency
    new RPCAddTorrent(service_, cq_, node_);

    std::string save_path = std::string("/home/lola/joystream");
    std::vector<char> resume_data = std::vector<char>();
    std::string name = std::string(request_.name());
    boost::optional<uint> upload_limit = -1;
    boost::optional<uint> download_limit = -1;
    bool paused = 1;
    joystream::core::TorrentIdentifier* torrent_identifier = joystream::core::TorrentIdentifier::fromHashString(std::string(request_.infohash()));

    std::cout << "We are adding the torrent" << std::endl;

    // if torrent_identifier not nullptr it means that hash was correct
    if (torrent_identifier != nullptr) {
        try {
            node_->addTorrent(upload_limit,download_limit,name,resume_data,save_path,paused,torrent_identifier,[this](libtorrent::error_code &ecode, libtorrent::torrent_handle &th) {
                if (ecode) {
                    std::cout << "We have an error" << std::endl;
                    this->finish(this->request_, false);
                } else {
                    std::cout << "New torrent added" << std::endl;
                    this->finish(this->request_, true);
                }
            });
        } catch (joystream::core::exception::TorrentAlreadyExists error) {
            std::cout << "Some kind of errors" << std::endl;
            this->finish(this->request_, false);
        }
    } else {
        this->finish(this->request_, false);
    }

}
