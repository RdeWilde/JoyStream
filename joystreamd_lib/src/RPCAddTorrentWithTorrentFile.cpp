#include <joystreamd_lib/RPCAddTorrentWithTorrentFile.hpp>
#include <typeinfo>

RPCAddTorrentWithTorrentFile::RPCAddTorrentWithTorrentFile(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : RPCRequestNormal(service, cq), node_(node)
{
    service_->RequestAddTorrentWithTorrentFile(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCAddTorrentWithTorrentFile::process()
{
    // Pop up a new instance for concurency
    new RPCAddTorrentWithTorrentFile(service_, cq_, node_);

    joystream::daemon::rpc::Void response;
    std::vector<unsigned char> data(request_.data().begin(), request_.data().end());
    joystream::core::TorrentIdentifier* torrent_identifier = joystream::core::TorrentIdentifier::fromTorrentFileContents(data);
    std::string save_path = std::string("/home/lola/joystream");
    std::vector<char> resume_data = std::vector<char>();
    std::string name;
    boost::optional<uint> upload_limit = -1;
    boost::optional<uint> download_limit = -1;
    bool paused = 0;

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
            std::cout << "Some kind of errors" << std::endl;
            this->finish(response, false);
        }
    } else {
        this->finish(response, false);
    }
}
