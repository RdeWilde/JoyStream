#include <joystreamd_lib/RPCBuyTorrent.hpp>

RPCBuyTorrent::RPCBuyTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::appkit::AppKit* appKit)
    : RPCRequestNormal(service, cq), appKit_(appKit)
{
    service_->RequestBuyTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCBuyTorrent::checkStatus(libtorrent::torrent_status::state_t state, float progress) {
    std::cout << state << std::endl;
    joystream::daemon::rpc::Void response;

    std::cout << "STATUS UPDATED" << std::endl;

    if (state == libtorrent::torrent_status::state_t::downloading) {
        std::cout << "We can buy people !" << std::endl;
        this->finish(response, true);
    }
}

void RPCBuyTorrent::process()
{
    // Pop up a new instance for concurency
    new RPCBuyTorrent(service_, cq_, appKit_);

    joystream::daemon::rpc::Void response;

    const int nsellers = 1;
    const uint64_t price = 100; //satoshis per piece

    const uint32_t locktime = 5;
    const uint64_t settlement_fee = 5000;
    const uint64_t contractFeeRate = 20000;

    const double secondsBeforeCreatingContract = 3;
    const double secondsBeforePieceTimeout = 25;

    joystream::protocol_session::BuyingPolicy buyingPolicy(secondsBeforeCreatingContract,
                                                           secondsBeforePieceTimeout,
                                                           joystream::protocol_wire::SellerTerms::OrderingPolicy::min_price);

    joystream::protocol_wire::BuyerTerms buyerTerms(price, locktime, nsellers, contractFeeRate);

    joystream::core::Torrent* torrent;

    std::cout << "We are looking for the torrent in the node" << std::endl;

    torrent = appKit_->node()->torrent(joystream::appkit::util::sha1_hash_from_hex_string(request_.infohash().c_str()));

    if (torrent != nullptr) {
        // We have found the torrent that we want to buy in the Node
        libtorrent::torrent_status::state_t torrentState = torrent->state();
        std::cout << torrentState << std::endl;

        if (torrentState == libtorrent::torrent_status::state_t::downloading) {
            try {
                appKit_->buyTorrent(20000, torrent, buyingPolicy, buyerTerms, [](const std::exception_ptr &e){
                    std::cout << "We are buying the torrent" << std::endl;
                });
            } catch(const std::runtime_error& error) {
                std::cout << error.what() << std::endl;
                this->finish(response, false);
            }
        } else {
            std::cout << "Need to wait for it to be on downloading state" << std::endl;
            QObject::connect(torrent, &joystream::core::Torrent::stateChanged, this, &RPCBuyTorrent::checkStatus);
        }

    } else {
        // Torrent not found in Node return error
        this->finish(response, false);
        return;
    }

}
