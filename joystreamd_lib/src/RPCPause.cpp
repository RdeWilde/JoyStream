#include <joystreamd_lib/RPCPause.hpp>

RPCPause::RPCPause(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : RPCRequestNormal(service, cq), node_(node)
{
    service_->RequestPause(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCPause::process()
{
    new RPCPause(service_, cq_, node_);

    node_->pause([this](){

        joystream::daemon::rpc::Void response;

        std::cout << "Node was paused" << std::endl;
        this->finish(response, true);
    });
}
