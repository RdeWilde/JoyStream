#include <joystreamd_lib/RPCRequest.hpp>


RPCRequest::RPCRequest(grpc::ServerCompletionQueue* cq)
    : cq_(cq), status_(READY)
{}

void RPCRequest::eventCompleted(bool fok)
{
    if (!fok) {
        delete this;
    } else {
        if (ctx_.IsCancelled()) {
            std::cout << "Request Cancelled" << std::endl;
            delete this;
        }
        if (status_ == READY) {
            status_ = PROCESSING;
            std::cout << "PROCESSING" << std::endl;
            process();
        } else {
            if (status_ == FINISHED) {
                std::cout << "DELETE" << std::endl;
                delete this;
            }
        }
    }
}
