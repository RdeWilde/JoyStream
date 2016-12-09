#include <joystreamd_lib/RPCSellTorrent.hpp>

RPCSellTorrent::RPCSellTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::appkit::AppKit* appKit)
    : RPCRequestNormal(service, cq), appKit_(appKit)
{
    service_->RequestSellTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCSellTorrent::sellingModeActivated()
{
    joystream::daemon::rpc::Void response;

    std::cout << "Selling Torrent" << std::endl;
    this->finish(response, true);
}

void RPCSellTorrent::process()
{
    new RPCSellTorrent(service_, cq_, appKit_);

    joystream::protocol_session::SellingPolicy sellingPolicy;
    libtorrent::sha1_hash infohash = joystream::appkit::util::sha1_hash_from_hex_string(request_.infohash().c_str());

    joystream::protocol_wire::SellerTerms sellerTerms(request_.minprice(), request_.minlock(), request_.maxsellers(), request_.mincontractfeeperkb(), request_.settlementfee());
    //joystream::protocol_wire::SellerTerms sellerTerms(100,5,1,20000,5000);

    auto sellResult = appKit_->sellTorrent(infohash, sellingPolicy, sellerTerms, joystream::protocol_session::SessionState::started);
    QObject::connect(sellResult.get(), &joystream::appkit::WorkerResult::finished, [this, sellResult](){
        joystream::daemon::rpc::Void response;

        if(sellResult->getError() != joystream::appkit::WorkerResult::Error::NoError) {
            this->finish(response, false);
            return;
        }
        this->finish(response, true);
    });
    /* Not working
    QObject::connect(sellResult.get(), &joystream::appkit::WorkerResult::finished,
                    this, &RPCSellTorrent::sellingModeActivated);*/

}
