#ifndef RPCREQUEST_H
#define RPCREQUEST_H

#include <QObject>
#include <QMetaObject>
#include <QApplication>
#include <QCoreApplication>


/**
 * @brief Abstract Class for rpc method
 *
 **/
class RPCRequest : public QObject {
    Q_OBJECT

    public:
        virtual ~RPCRequest() {}
        Q_INVOKABLE virtual void proceed(bool fok) = 0;
        //virtual void onCall() = 0;

        enum CallStatus { READY, FINISH };
};

#endif // RPCREQUEST_H
