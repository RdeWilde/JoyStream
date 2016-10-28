#include "AsyncCallHandler.h"

AsyncCallHandler::AsyncCallHandler(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node)
    : service_(service), cq_(cq)
{
    std::cout << "AsyncCallHandler created" << std::endl;
    new RPCTest(service, cq_);
    new RPCPause(service, cq_, node);
}

AsyncCallHandler::~AsyncCallHandler()
{
    std::cout << "AsyncCallHandler destroyed" << std::endl;
}

void AsyncCallHandler::run()
{
  void * tag;
  bool fok;
  RPCRequest* call;

  /*for ( auto rpc = rpcs_.begin(); rpc != rpcs_.end(); ++rpc ) {
      // We don't need this
  }*/

  while ( cq_->Next(&tag, &fok) ) {
    std::cout << "In the loop" << std::endl;
    call = static_cast<RPCRequest *>(tag);
    call->moveToThread(QApplication::instance()->thread());
    QMetaObject::invokeMethod(call, "proceed", Qt::QueuedConnection, Q_ARG(bool, fok), Q_ARG(void*, tag));
  }
}
