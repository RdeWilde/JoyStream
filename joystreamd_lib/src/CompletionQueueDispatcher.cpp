#include <joystreamd_lib/CompletionQueueDispatcher.hpp>

CompletionQueueDispatcher::CompletionQueueDispatcher() {
    std::cout << "AsyncCallHandler created" << std::endl;
}

CompletionQueueDispatcher::~CompletionQueueDispatcher()
{
    std::cout << "AsyncCallHandler destroyed" << std::endl;
}

void CompletionQueueDispatcher::run(grpc::ServerCompletionQueue* cq)
{
  void * tag;
  bool fok;
  RPCRequest* call;

  while ( cq->Next(&tag, &fok) ) {
    std::cout << "In the loop" << std::endl;
    call = static_cast<RPCRequest *>(tag);
    QMetaObject::invokeMethod(call, "eventCompleted", Qt::QueuedConnection, Q_ARG(bool, fok));
  }
  std::cout << "Out of the loop" << std::endl;
}