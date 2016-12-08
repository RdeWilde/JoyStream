#include <joystreamd_lib/RPCBuyTorrent.hpp>

RPCBuyTorrent::RPCBuyTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::appkit::AppKit* appKit)
    : RPCRequestNormal(service, cq), appKit_(appKit)
{
    service_->RequestBuyTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCBuyTorrent::checkStatus(libtorrent::torrent_status::state_t state, float progress) {
    joystream::daemon::rpc::Void response;

    joystream::protocol_session::BuyingPolicy buyingPolicy(request_.secondsbeforecreatingcontract(),
                                                           request_.secondsbeforepiecetimeout(),
                                                           joystream::protocol_wire::SellerTerms::OrderingPolicy::min_price);

    joystream::protocol_wire::BuyerTerms buyerTerms(request_.price(), request_.locktime(), request_.nsellers(), request_.contractfeerate());

    joystream::core::Torrent* torrent;

    torrent = appKit_->node()->torrent(joystream::appkit::util::sha1_hash_from_hex_string(request_.infohash().c_str()));

    if (state == libtorrent::torrent_status::state_t::downloading) {

        bool hasPlugin = torrent->torrentPluginSet();

        if (hasPlugin)
        {
            try {
                /*appKit_->buyTorrent(torrent->infoHash(), buyingPolicy, buyerTerms, [this, response, torrent](const std::exception_ptr &e){
                    std::cout << "We are buying the torrent" << std::endl;
                    torrent->torrentPlugin()->start([this, response](const std::exception_ptr &e){
                        std::cout << "We started pluggin" << std::endl;
                        this->finish(response, true);
                    });
                });*/
                appKit_->buyTorrent(torrent->infoHash(), buyingPolicy, buyerTerms);
                std::cout << "We are buying the torrent" << std::endl;
                torrent->torrentPlugin()->start([this, response](const std::exception_ptr &e){
                    std::cout << "We started pluggin" << std::endl;
                    this->finish(response, true);
                });
            } catch(const std::runtime_error& error) {
                std::cout << error.what() << std::endl;
                this->finish(response, false);
            }
        } else {
            std::cout << "No plugin..." << std::endl;
        }
    } else {
        std::cout << "Not downloading..." << std::endl;
    }
}

void RPCBuyTorrent::pluginAdded(joystream::core::TorrentPlugin* torrentPlugin) {
    joystream::daemon::rpc::Void response;

    joystream::protocol_session::BuyingPolicy buyingPolicy(request_.secondsbeforecreatingcontract(),
                                                           request_.secondsbeforepiecetimeout(),
                                                           joystream::protocol_wire::SellerTerms::OrderingPolicy::min_price);

    joystream::protocol_wire::BuyerTerms buyerTerms(request_.price(), request_.locktime(), request_.nsellers(), request_.contractfeerate());

    joystream::core::Torrent* torrent;

    std::cout << "PLUGIN ADDED" << std::endl;

    torrent = appKit_->node()->torrent(joystream::appkit::util::sha1_hash_from_hex_string(request_.infohash().c_str()));

    if (torrent->state() == libtorrent::torrent_status::state_t::downloading) {

        bool hasPlugin = torrent->torrentPluginSet();

        if (hasPlugin)
        {
            std::cout << "Ok we have the plugin set" << std::endl;
            try {
                /*appKit_->buyTorrent(torrent->infoHash(), buyingPolicy, buyerTerms, [this, response, torrent](const std::exception_ptr &e){
                    std::cout << "We are buying the torrent" << std::endl;
                    torrent->torrentPlugin()->start([this, response](const std::exception_ptr &e){
                        std::cout << "We started pluggin" << std::endl;
                        this->finish(response, true);
                    });
                });*/
                appKit_->buyTorrent(torrent->infoHash(), buyingPolicy, buyerTerms);
                std::cout << "We are buying the torrent" << std::endl;
                torrent->torrentPlugin()->start([this, response](const std::exception_ptr &e){
                    std::cout << "We started pluggin" << std::endl;
                    this->finish(response, true);
                });
            } catch(const std::runtime_error& error) {
                std::cout << error.what() << std::endl;
                this->finish(response, false);
            }
        } else {
            std::cout << "No plugin..." << std::endl;
        }
    } else {
        std::cout << "Not downloading..." << std::endl;
    }
}

void RPCBuyTorrent::process()
{
    // Pop up a new instance for concurency
    new RPCBuyTorrent(service_, cq_, appKit_);

    joystream::daemon::rpc::Void response;


    joystream::protocol_session::BuyingPolicy buyingPolicy(request_.secondsbeforecreatingcontract(),
                                                           request_.secondsbeforepiecetimeout(),
                                                           joystream::protocol_wire::SellerTerms::OrderingPolicy::min_price);

    joystream::protocol_wire::BuyerTerms buyerTerms(request_.price(), request_.locktime(), request_.nsellers(), request_.contractfeerate());

    joystream::core::Torrent* torrent;

    torrent = appKit_->node()->torrent(joystream::appkit::util::sha1_hash_from_hex_string(request_.infohash().c_str()));

    if (torrent != nullptr) {
        // We have found the torrent that we want to buy in the Node
        libtorrent::torrent_status::state_t torrentState = torrent->state();
        std::cout << torrentState << std::endl;

        if (torrentState == libtorrent::torrent_status::state_t::downloading) {
            try {
                /*appKit_->buyTorrent(20000, torrent, buyingPolicy, buyerTerms, [this, response, torrent](const std::exception_ptr &e){
                    std::cout << "We are buying the torrent" << std::endl;
                    torrent->torrentPlugin()->start([this, response](const std::exception_ptr &e){
                        std::cout << "We started pluggin" << std::endl;
                        this->finish(response, true);
                    });
                });*/
                appKit_->buyTorrent(torrent->infoHash(), buyingPolicy, buyerTerms);
                std::cout << "We are buying the torrent" << std::endl;
                torrent->torrentPlugin()->start([this, response](const std::exception_ptr &e){
                    std::cout << "We started pluggin" << std::endl;
                    this->finish(response, true);
                });
            } catch(const std::runtime_error& error) {
                std::cout << error.what() << std::endl;
                this->finish(response, false);
            }
        } else {
            std::cout << "Need to wait for it to be on downloading state" << std::endl;
            QObject::connect(torrent, &joystream::core::Torrent::stateChanged, this, &RPCBuyTorrent::checkStatus);
            QObject::connect(torrent, &joystream::core::Torrent::torrentPluginAdded, this, &RPCBuyTorrent::pluginAdded);
        }

    } else {
        // Torrent not found in Node return error
        std::cout << "Torrent not found" << std::endl;
        this->finish(response, false);
    }

}
