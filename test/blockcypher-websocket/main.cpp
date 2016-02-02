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
                     [&client](const BlockCypher::TX & tx){

        std::cout << tx.toTransaction().hash().getHex() << std::endl;
        //client.disconnect();
    });

    QObject::connect(&client, &BlockCypher::WebSocketClient::connected, [](){
        std::cout << "CONNECTED\n";
    });

    QObject::connect(&client, &BlockCypher::WebSocketClient::disconnected, [&a](){
        std::cout << "DISCONNECTED\n";
        a.exit();
    });

    QObject::connect(&client, &BlockCypher::WebSocketClient::error, [](QAbstractSocket::SocketError e){
        std::cout << "WEB SOCKET ERROR: " << e << std::endl;
    });


    QObject::connect(&client, &BlockCypher::WebSocketClient::parseError, [](QString e){
        std::cout << "PARSE ERROR: " << e.toStdString() << std::endl;
    });

    QObject::connect(&client, &BlockCypher::WebSocketClient::apiError, [](QString e){
        std::cout << "API ERROR: " << e.toStdString() << std::endl;
    });

    BlockCypher::Event ev(BlockCypher::Event::Type::unconfirmed_tx);

    client.addEvent(ev);

    std::cout << "connecting...\n";

    client.connect();

    return a.exec();
}

