#include <joystreamd_lib/RPCBuyTorrent.hpp>

RPCBuyTorrent::RPCBuyTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::appkit::AppKit* appKit)
    : RPCRequestNormal(service, cq), appKit_(appKit)
{
    service_->RequestBuyTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
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

    const joystream::core::Torrent* torrent;

    const double secondsBeforeCreatingContract = 3;
    const double secondsBeforePieceTimeout = 25;

    joystream::protocol_session::BuyingPolicy buyingPolicy(secondsBeforeCreatingContract,
                                                           secondsBeforePieceTimeout,
                                                           joystream::protocol_wire::SellerTerms::OrderingPolicy::min_price);

    joystream::protocol_wire::BuyerTerms buyerTerms(price, locktime, nsellers, contractFeeRate);

    std::cout << "BUYING TORRENT" << std::endl;

    for (const auto t : appKit_->node()->torrents()) {
        if (libtorrent::to_hex(t.first.to_string()) == request_.infohash()) {
            // We've found the torrent to buy.
            try {
                appKit_->buyTorrent(20000, t.second, buyingPolicy, buyerTerms, [](const std::exception_ptr &e){
                    std::cout << "We are buying the torrent" << std::endl;
                });
            } catch(std::runtime_error) {
                this->finish(response, false);
            }
            break;
        }
    }

    //appKit_->buyTorrent();

}
