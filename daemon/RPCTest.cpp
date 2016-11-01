#include "RPCTest.h"

RPCTest::RPCTest(joystream::daemon::rpc::Daemon::AsyncService * service, grpc::ServerCompletionQueue* cq)
    : service_(service), cq_(cq), responder_(&ctx_), status_(READY)
{
    service_->Requesttest1(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCTest::proceed(bool fok)
{
    // If not a regular event we delete
    if (!fok) {
        delete this;
    } else {
        if (status_ == READY) {

            new RPCTest(service_, cq_);

            std::cout << request_.clientmessage().c_str() << std::endl;
            responce_.set_servermessage("This is a server message");
            responder_.Finish(responce_, grpc::Status::OK, this);
            status_ = FINISH;

        } else {
            std::cout << "Destroy RPCTest" << std::endl;
            delete this;
        }
    }
}
