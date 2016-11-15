#include <QCoreApplication>
#include <unistd.h>

#include <app_kit/kit.hpp>

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

  joystream::appkit::DataDirectory dataDir(std::string("/home/lola/joystream/test/"));

  // Create the objects we need here to pass to the daemon
  auto kit = joystream::core::AppKit::create(dataDir.walletFilePath().toStdString(),
                                             dataDir.blockTreeFilePath().toStdString(),
                                             TEST_BITCOIN_NETWORK);


  ServerImpl server(node, &a);

  server_ = &server;

  signal(SIGINT, signal_handler);

  return  a.exec();
}
