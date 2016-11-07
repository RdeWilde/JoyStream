#include "RPCPauseTorrent.h"

RPCPauseTorrent::RPCPauseTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : service_(service), cq_(cq), node_(node), responder_(&ctx_), RPCRequest()
{
    //service_->RequestPauseTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCPauseTorrent::onCall()
{
    joystream::daemon::rpc::Void response;

    new RPCPauseTorrent(service_, cq_, node_);

    responder_.Finish(response, grpc::Status::OK, this);
    status_ = FINISH;

}
