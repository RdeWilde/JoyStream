#include <joystreamd_lib/CompletionQueueDispatcher.hpp>

CompletionQueueDispatcher::CompletionQueueDispatcher()
    : thread_()
{
    std::cout << "AsyncCallHandler created" << std::endl;
}

CompletionQueueDispatcher::~CompletionQueueDispatcher()
{
    std::cout << "AsyncCallHandler destroyed" << std::endl;
    if(thread_.joinable()) {
        std::cout << "Thread joinable" << std::endl;
        thread_.join();
        std::cout << "Thread joined" << std::endl;
    }
}

void CompletionQueueDispatcher::StartDispatcher(grpc::ServerCompletionQueue* cq)
{
    thread_ = std::thread(&CompletionQueueDispatcher::run,this,cq);
}


void CompletionQueueDispatcher::run(grpc::ServerCompletionQueue* cq)
{
  void * tag;
  bool fok;
  RPCRequest* call;

  while ( cq->Next(&tag, &fok) ) {
    call = static_cast<RPCRequest *>(tag);
    std::cout << "In the loop" << std::endl;
    QMetaObject::invokeMethod(call, "eventCompleted", Qt::QueuedConnection, Q_ARG(bool, fok));
  }
}
