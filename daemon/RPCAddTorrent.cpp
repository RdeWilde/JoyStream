#include "RPCAddTorrent.h"

RPCAddTorrent::RPCAddTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : service_(service), cq_(cq), node_(node), responder_(&ctx_), status_(READY)
{
    service_->RequestAddTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCAddTorrent::proceed(bool fok)
{
    if (!fok) {
        delete this;
    } else {
        if (status_ == READY) {

            new RPCAddTorrent(service_, cq_, node_);

            libtorrent::sha1_hash info_hash = libtorrent::sha1_hash(request_.infohash());
            std::string save_path = std::string("/home/lola/joystream");
            std::vector<char> resume_data = std::vector<char>();
            std::string name = std::string(request_.name());
            boost::optional<uint> upload_limit = -1;
            boost::optional<uint> download_limit = -1;
            bool paused = 1;
            joystream::core::TorrentIdentifier torrent_identifier = joystream::core::TorrentIdentifier(info_hash);
            std::cout << "We are adding the torrent" << std::endl;
            std::cout << request_.infohash() << std::endl;
            node_->addTorrent(upload_limit,download_limit,name,resume_data,save_path,paused,torrent_identifier,[this](libtorrent::error_code &ecode, libtorrent::torrent_handle &th) {
                std::cout << "New torrent added" << std::endl;
                this->responder_.Finish(this->request_, grpc::Status::OK, this);
                this->status_ = FINISH;
            });

        } else {

            std::cout << "Delete RPCPause instance" << std::endl;
            delete this;

        }
    }
}
