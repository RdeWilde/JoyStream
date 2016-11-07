#include "RPCPause.h"

RPCPause::RPCPause(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : service_(service), cq_(cq), node_(node), responder_(&ctx_), RPCRequest()
{
    service_->RequestPause(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCPause::onCall()
{
    new RPCPause(service_, cq_, node_);

    std::cout << "We want to pause the node" << std::endl;
    node_->pause([this](){

        joystream::daemon::rpc::Void response;

        std::cout << "Node was paused" << std::endl;
        this->responder_.Finish(response, grpc::Status::OK, this);
        this->status_ = FINISH;
    });
}
