#include <QCoreApplication>
#include <core/Node.hpp>

#include <boost/asio/impl/src.hpp>

#include "ServerImpl.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  // Create the objects we need here to pass to the daemon
  auto node = joystream::core::Node::create([](const Coin::Transaction &tx){
    // broadcast tx
  });

  ServerImpl server(node);

  return a.exec();
}
