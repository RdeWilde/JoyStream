#include <QCoreApplication>
#include <unistd.h>
#include <stdio.h>

#include <app_kit/kit.hpp>
#include <common/Network.hpp>

#include <boost/asio/impl/src.hpp>

#include <joystreamd_lib/ServerImpl.hpp>

#define TEST_BITCOIN_NETWORK Coin::Network::testnet3

ServerImpl* server_;

void signal_handler(int num)
{
    std::cout << "Stop" << std::endl;
    server_->Shutdown();
}

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  // Default value
  std::string path = std::string("/home/lola/joystream/test/");
  std::string port = "3000";

  int opt;
  while ((opt = getopt (argc, argv, "p:f:")) != -1) {
    switch (opt) {
        case 'p':
            std::cout << "Server is going to start on port : " << optarg << std::endl;
            port = optarg;
            break;
        case 'f':
            std::cout << "Joystream folder path :" << optarg << std::endl;
            path = std::string(optarg);
            break;
        default:
            std::cout << "Unknown argument" << std::endl;
            break;
    }
  }

  joystream::appkit::DataDirectory dataDir(path);

  // Create the objects we need here to pass to the daemon
  auto kit = joystream::appkit::AppKit::create(dataDir.walletFilePath().toStdString(),
                                             dataDir.blockTreeFilePath().toStdString(),
                                             TEST_BITCOIN_NETWORK);

  ServerImpl server(kit, &a, port, path);

  server_ = &server;

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  return  a.exec();
}
