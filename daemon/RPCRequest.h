#ifndef RPCREQUEST_H
#define RPCREQUEST_H

#include <QObject>
#include <QMetaObject>
#include <QApplication>
#include <QCoreApplication>


/////////////////////////////////
/// \brief The RPCRequest class
/// Abstract Class
///
class RPCRequest : public QObject {
    Q_OBJECT

    public:
        virtual ~RPCRequest() {}
        Q_INVOKABLE virtual void proceed(bool fok, void* tag) = 0;

        enum CallStatus { READY, FINISH };
};

#endif // RPCREQUEST_H
