#include <QCoreApplication>
#include <unistd.h>

#include <app_kit/kit.hpp>
#include <common/Network.hpp>

#include <boost/asio/impl/src.hpp>

#include <joystreamd_lib/ServerImpl.hpp>

#define TEST_BITCOIN_NETWORK Coin::Network::testnet3

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
  auto kit = joystream::appkit::AppKit::create(dataDir.walletFilePath().toStdString(),
                                             dataDir.blockTreeFilePath().toStdString(),
                                             TEST_BITCOIN_NETWORK);


  ServerImpl server(kit->node(), kit->wallet(), &a);

  server_ = &server;

  signal(SIGINT, signal_handler);

  return  a.exec();
}
