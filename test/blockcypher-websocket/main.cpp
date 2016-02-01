#include <QCoreApplication>

#include <CoinCore/CoinNodeData.h>
#include <common/Network.hpp>


#include <blockcypher/TX.hpp>
#include <blockcypher/WebSocketClient.hpp>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BlockCypher::WebSocketClient client(Coin::Network::mainnet);

    QObject::connect(&client, &BlockCypher::WebSocketClient::txArrived,
                     [](const BlockCypher::TX & tx, BlockCypher::Event::Type type){

        std::cout << tx.toTransaction().toString() << std::endl;
    });

    QObject::connect(&client, &BlockCypher::WebSocketClient::connected, [](){
        std::cout << "CONNECTED\n";
    });

    QObject::connect(&client, &BlockCypher::WebSocketClient::error, [](QAbstractSocket::SocketError e){
        std::cout << "WEB SOCKET ERROR: " << e << std::endl;
    });


    QObject::connect(&client, &BlockCypher::WebSocketClient::parseError, [](QString e){
        std::cout << "ERROR: " << e.toStdString() << std::endl;
    });

    client.addEvent(BlockCypher::Event(BlockCypher::Event::Type::unconfirmed_tx));

    std::cout << "connecting...\n";

    client.connect();

    return a.exec();
}

