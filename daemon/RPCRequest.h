#ifndef RPCREQUEST_H
#define RPCREQUEST_H

#include <QObject>
#include <QMetaObject>
#include <QApplication>
#include <QCoreApplication>

#include "protos/daemon.grpc.pb.h"
#include "protos/daemon.pb.h"

/**
 * @brief Abstract Class for rpc method
 *
 **/
class RPCRequest : public QObject {
    Q_OBJECT

    public:
        RPCRequest();
        Q_INVOKABLE void proceed(bool fok);
        virtual void onCall() = 0;

        enum CallStatus { READY, FINISH };

    protected:
        CallStatus status_;
        grpc::ServerContext ctx_;
};

#endif // RPCREQUEST_H
