#include <joystreamd_lib/RPCAddTorrent.hpp>

RPCAddTorrent::RPCAddTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::appkit::AppKit *appKit, std::string defaultSavePath)
    : RPCRequestNormal(service, cq), appKit_(appKit), defaultSavePath_(defaultSavePath)
{
    service_->RequestAddTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

RPCAddTorrent::torrentAdded()
{
    joystream::daemon::rpc::Void response;

    /*if(addResult->getError() != joystream::appkit::WorkerResult::Error::NoError) {
        this->finish(response, false);
        return;
    }*/
    std::cout << "Torrent Added" << std::endl;
    this->finish(response, true);
}

void RPCAddTorrent::process()
{

    new RPCAddTorrent(service_, cq_, appKit_, defaultSavePath_);

    joystream::daemon::rpc::Void response;

    std::string save_path = defaultSavePath_;
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
    if (torrent_identifier) {
        std::cout << "Torrent InfoHash: " << torrent_identifier->infoHash() << std::endl;

        auto addResult = appKit_->addTorrent(*torrent_identifier, save_path);
        delete torrent_identifier;
        QObject::connect(addResult.get(), &joystream::appkit::WorkerResult::finished, this, &RPCAddTorrent::torrentAdded);
    } else {
        this->finish(response, false);
    }

}
