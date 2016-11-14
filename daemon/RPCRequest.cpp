#include "RPCRequest.h"


RPCRequest::RPCRequest(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq)
    : service_(service), cq_(cq), status_(READY)
{}

void RPCRequest::proceed(bool fok)
{
    if (!fok) {
        // Need to verify if it is not PROCESSING a call
        // If it is will need to wait until FINISHED because it might
        // be in a callback function in node.
        this->deleteLater();
    } else {
        if (status_ == READY) {
            status_ = PROCESSING;
            onCall();
        } else {
            delete this;
        }
    }
}
