#ifndef RPCREQUESTSTREAMSERVER_H
#define RPCREQUESTSTREAMSERVER_H


#include "RPCRequest.h"

/**
 * @brief Template class for rpc call with a server side stream
 *
 **/
template <class RESP>
class RPCRequestStreamServer : public RPCRequest {

    public:
        RPCRequestStreamServer(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq)
            : RPCRequest(service, cq), responder_(&ctx_) {}

        /**
         * @brief Will answer to the client
         * and update the status_ to FINISHED.
         * Only for Stream server side RPC.
         * */
        void finish(bool success)
        {
            if (success) {
                responder_.Finish(grpc::Status::OK, this);
            } else {
                responder_.Finish(grpc::Status::CANCELLED, this);
            }
            status_ = FINISHED;
        }

        /**
         * @brief Write answer to the client (only for Stream)
         * */
        void write(const RESP &response)
        {
            responder_.Write(response, this);
            status_ = WRITING;
        }

    protected:
        grpc::ServerAsyncWriter<RESP> responder_;

    private:
        using RPCRequest::status_;

};

#endif // RPCREQUESTSTREAMSERVER_H
