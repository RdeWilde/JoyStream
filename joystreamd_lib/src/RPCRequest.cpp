#include <joystreamd_lib/RPCRequest.hpp>


RPCRequest::RPCRequest(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq)
    : service_(service), cq_(cq), status_(READY)
{}

void RPCRequest::eventCompleted(bool fok)
{
    if (!fok) {
        delete this;
    } else {
        if (status_ == READY) {
            status_ = PROCESSING;
            process();
        } else {
            delete this;
        }
    }
}
