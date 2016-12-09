#include <joystreamd_lib/RPCSellTorrent.hpp>

RPCSellTorrent::RPCSellTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::appkit::AppKit* appKit)
    : RPCRequestNormal(service, cq), appKit_(appKit)
{
    service_->RequestSellTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCSellTorrent::process()
{
    // Pop up a new instance for concurency
    new RPCSellTorrent(service_, cq_, appKit_);

    std::shared_ptr<joystream::appkit::WorkerResult> workerResult;

    joystream::protocol_session::SellingPolicy sellingPolicy;

    joystream::protocol_wire::SellerTerms sellerTerms(request_.minprice(), request_.minlock(), request_.maxsellers(), request_.mincontractfeeperkb(), request_.settlementfee());

    workerResult = appKit_->sellTorrent(libtorrent::sha1_hash(request_.infohash().c_str()), sellingPolicy, sellerTerms);
    std::cout << "We are selling the torrent" << std::endl;
    QObject::connect(workerResult.get(), &joystream::appkit::WorkerResult::finished, this, [this](){
        joystream::daemon::rpc::Void response;

        this->finish(response, true);
    });

}
