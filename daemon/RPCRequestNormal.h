#ifndef RPCREQUESTNORMAL_H
#define RPCREQUESTNORMAL_H

#include "RPCRequest.h"


/**
 * @brief Template class for all normal rpc call
 *
 **/
template <class RESP>
class RPCRequestNormal : public RPCRequest {

    public:
        RPCRequestNormal(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq)
            : RPCRequest(service, cq), responder_(&ctx_) {}


        /**
         * @brief Will answer to the client
         * and update the status_ to FINISHED.
         * Only for normal RPC (no Stream).
         * */
        void finish(const RESP &response, bool success )
        {
            if (success) {
                responder_.Finish(response, grpc::Status::OK, this);
            } else {
                responder_.Finish(response, grpc::Status::CANCELLED, this);
            }
            status_ = FINISHED;
        }

    protected:
        grpc::ServerAsyncResponseWriter<RESP> responder_;

    private:
        using RPCRequest::status_;

};
#endif // RPCREQUESTNORMAL_H
