#include <joystreamd_lib/RPCRequest.hpp>


RPCRequest::RPCRequest(grpc::ServerCompletionQueue* cq)
    : cq_(cq), status_(READY)
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
            if (status_ == FINISHED) {
                delete this;
            }
        }
    }
}
