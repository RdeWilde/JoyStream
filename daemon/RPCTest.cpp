#include "RPCTest.h"

RPCTest::RPCTest(joystream::daemon::rpc::Daemon::AsyncService * service, grpc::ServerCompletionQueue* cq)
    : service_(service), cq_(cq), responder_(&ctx_), RPCRequest()
{
    service_->Requesttest1(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCTest::onCall()
{
    new RPCTest(service_, cq_);

    std::cout << request_.clientmessage().c_str() << std::endl;
    responce_.set_servermessage("This is a server message");
    responder_.Finish(responce_, grpc::Status::OK, this);
    status_ = FINISH;
}
