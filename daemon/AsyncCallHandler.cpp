#include "AsyncCallHandler.h"

AsyncCallHandler::AsyncCallHandler()
    : thread()
{
    std::cout << "AsyncCallHandler created" << std::endl;

    thread = std::thread(&AsyncCallHandler::run,this);
}

AsyncCallHandler::~AsyncCallHandler()
{
    std::cout << "AsyncCallHandler destroyed" << std::endl;
}

void AsyncCallHandler::setCompletionQueue(grpc::ServerCompletionQueue* cq)
{
  cq_ = cq;
}

void AsyncCallHandler::run()
{
  void * tag;
  bool fok;
  RPCRequest* call;

  while ( cq_->Next(&tag, &fok) ) {
    std::cout << "In the loop" << std::endl;
    call = static_cast<RPCRequest *>(tag);
    QMetaObject::invokeMethod(call, "proceed", Qt::QueuedConnection, Q_ARG(bool, fok));
  }
}
