#include <joystreamd_lib/RPCSellTorrent.hpp>

RPCSellTorrent::RPCSellTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::appkit::AppKit* appKit)
    : RPCRequestNormal(service, cq), appKit_(appKit)
{
    service_->RequestSellTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCSellTorrent::checkStatus(libtorrent::torrent_status::state_t state, float progress) {
    joystream::daemon::rpc::Void response;

    joystream::protocol_session::SellingPolicy sellingPolicy;

    joystream::protocol_wire::SellerTerms sellerTerms(22, 134, 10, 88, 32);

    joystream::core::Torrent* torrent;

    torrent = appKit_->node()->torrent(joystream::appkit::util::sha1_hash_from_hex_string(request_.infohash().c_str()));

    if (state == libtorrent::torrent_status::state_t::seeding) {
        bool hasPlugin = torrent->torrentPluginSet();

        if (hasPlugin)
        {
            try {
                appKit_->sellTorrent(torrent, sellingPolicy, sellerTerms, [this, response, torrent](const std::exception_ptr &e){
                    std::cout << "We are selling the torrent" << std::endl;
                    torrent->torrentPlugin()->start([this, response](const std::exception_ptr &e){
                        std::cout << "We started pluggin" << std::endl;
                        this->finish(response, true);
                    });
                });
            } catch(const std::runtime_error& error) {
                std::cout << error.what() << std::endl;
                this->finish(response, false);
            }
        } else {
            std::cout << "No plugin..." << std::endl;
        }
    } else {
        std::cout << "No seeding..." << std::endl;
    }
}

void RPCSellTorrent::pluginAdded(joystream::core::TorrentPlugin* torrentPlugin) {
    joystream::daemon::rpc::Void response;

    joystream::protocol_session::SellingPolicy sellingPolicy;

    joystream::protocol_wire::SellerTerms sellerTerms(22, 134, 10, 88, 32);

    joystream::core::Torrent* torrent;

    std::cout << "PLUGIN ADDED" << std::endl;

    torrent = appKit_->node()->torrent(joystream::appkit::util::sha1_hash_from_hex_string(request_.infohash().c_str()));

    if (torrent->state() == libtorrent::torrent_status::state_t::seeding) {

        bool hasPlugin = torrent->torrentPluginSet();

        if (hasPlugin)
        {
            try {
                appKit_->sellTorrent(torrent, sellingPolicy, sellerTerms, [this, response, torrent](const std::exception_ptr &e){
                    std::cout << "We are selling the torrent" << std::endl;
                    torrent->torrentPlugin()->start([this, response](const std::exception_ptr &e){
                        std::cout << "We started pluggin" << std::endl;
                        this->finish(response, true);
                    });
                });
            } catch(const std::runtime_error& error) {
                std::cout << error.what() << std::endl;
                this->finish(response, false);
            }
        } else {
            std::cout << "No plugin..." << std::endl;
        }
    } else {
        std::cout << "Not selling..." << std::endl;
    }
}

void RPCSellTorrent::process()
{
    // Pop up a new instance for concurency
    new RPCSellTorrent(service_, cq_, appKit_);

    joystream::daemon::rpc::Void response;


    joystream::protocol_session::SellingPolicy sellingPolicy;

    joystream::protocol_wire::SellerTerms sellerTerms(22, 134, 10, 88, 32);

    joystream::core::Torrent* torrent;

    torrent = appKit_->node()->torrent(joystream::appkit::util::sha1_hash_from_hex_string(request_.infohash().c_str()));

    if (torrent != nullptr) {
        // We have found the torrent that we want to buy in the Node
        libtorrent::torrent_status::state_t torrentState = torrent->state();

        if (torrentState == libtorrent::torrent_status::state_t::seeding) {

            bool hasPlugin = torrent->torrentPluginSet();

            if (hasPlugin)
            {
                try {
                    appKit_->sellTorrent(torrent, sellingPolicy, sellerTerms, [this, response, torrent](const std::exception_ptr &e){
                        std::cout << "We are selling the torrent" << std::endl;
                        torrent->torrentPlugin()->start([this, response](const std::exception_ptr &e){
                            std::cout << "We started pluggin" << std::endl;
                            this->finish(response, true);
                        });
                    });
                } catch(const std::runtime_error& error) {
                    std::cout << error.what() << std::endl;
                    this->finish(response, false);
                }
            } else {
                std::cout << "Need to wait for it to be on seeding state" << std::endl;
                QObject::connect(torrent, &joystream::core::Torrent::stateChanged, this, &RPCSellTorrent::checkStatus);
                QObject::connect(torrent, &joystream::core::Torrent::torrentPluginAdded, this, &RPCSellTorrent::pluginAdded);
            }
        } else {
            std::cout << "No plugin..." << std::endl;
            QObject::connect(torrent, &joystream::core::Torrent::torrentPluginAdded, this, &RPCSellTorrent::pluginAdded);
        }

    } else {
        // Torrent not found in Node return error
        std::cout << "Torrent not found" << std::endl;
        this->finish(response, false);
    }

}
