#include <QCoreApplication>
#include <core/Node.hpp>
#include <unistd.h>

#include <boost/asio/impl/src.hpp>

#include <daemon/ServerImpl.hpp>

ServerImpl* server_;

void signal_handler(int num)
{
    std::cout << "Stop" << std::endl;
    delete server_;
}

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  // Create the objects we need here to pass to the daemon
  auto node = joystream::core::Node::create([](const Coin::Transaction &tx){
    // broadcast tx
  });


  ServerImpl server(node, &a);

  server_ = &server;

  signal(SIGINT, signal_handler);

  return  a.exec();
}
