#include "RPCPause.h"

RPCPause::RPCPause(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : service_(service), cq_(cq), node_(node), responder_(&ctx_), status_(READY)
{
    service_->RequestPause(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCPause::proceed(bool fok, void * tag)
{
    if (status_ == READY) {

        new RPCPause(service_, cq_, node_);

        std::cout << "We want to pause the node" << std::endl;
        node_->pause([this](){
            std::cout << "Node was paused" << std::endl;
            this->responder_.Finish(this->response_, grpc::Status::OK, this);
            this->status_ = FINISH;
        });

    } else {

        std::cout << "Delete RPCPause instance" << std::endl;
        delete this;

    }
}
