#ifndef RPCREQUEST_H
#define RPCREQUEST_H

#include <QObject>
#include <QMetaObject>
#include <QApplication>
#include <QCoreApplication>

#include "protos/daemon.grpc.pb.h"
#include "protos/daemon.pb.h"

enum CallStatus { READY, PROCESSING, WRITING, FINISHED };

/**
 * @brief Abstract Class for rpc call
 *
 **/
class RPCRequest : public QObject {
    Q_OBJECT

    public:
        RPCRequest(joystream::daemon::rpc::Daemon::AsyncService* service_, grpc::ServerCompletionQueue* cq_);

        /**
         * @brief will proceed the call or if the server is shutting down (fok=false)
         * will delete himself. Will also delete himself if we done with the call.
         * @param bool fok
         * */
        Q_INVOKABLE void proceed(bool fok);

        virtual void onCall() = 0;

    protected:
        grpc::ServerCompletionQueue* cq_;
        joystream::daemon::rpc::Daemon::AsyncService* service_;
        grpc::ServerContext ctx_;
        CallStatus status_;

};

#endif // RPCREQUEST_H
