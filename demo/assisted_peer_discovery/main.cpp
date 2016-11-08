#include <QApplication>
#include <QTimer>

#include <boost/asio/impl/src.hpp> // Problem 1
#include <core/core.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <iostream>
#include <functional>
using namespace joystream;
using namespace joystream::core;
using namespace std;

// Problem 2:
// Explicit template instantiation of IdToString()
// used in joystream::protocol_session::exception::ConnectionAlreadyAddedException
template <>
std::string IdToString<boost::asio::ip::basic_endpoint<boost::asio::ip::tcp>>(boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> const&id){
    return id.address().to_string();
}

vector<Node*> nodes;
QTimer addNodeTimer;
QTimer printJSNodesTimer;

void printJSNodes() {
    for(int i=0; i<nodes.size(); i++) {
        Node &node = *nodes[i];

        for(auto &pair : node.torrents()) {
            Torrent &t = *pair.second;
            for(auto &pair2 : t.announcedJSPeersAtTimestamp()) {
                auto &endpoint = pair2.first;
                cout << "Node " << (i+1) << " has a JS peer: " << endpoint.address() << ":" << endpoint.port() << endl;
            }
        }
    }
}

void addNode() {
    cout << "    Adding node " << nodes.size()+1 << endl;

    nodes.push_back(Node::create(Node::BroadcastTransaction()));
    Node &node = *nodes.back();
    TorrentIdentifier identifier(libtorrent::sha1_hash("0a4193f50658c7f195288bfd84a1b067697e21a2")); // Ubuntu 16.04 I think
    node.setAssistedPeerDiscovery(true);
    node.addTorrent(boost::optional<uint>(),
                    boost::optional<uint>(),
                    "", std::vector<char>(),
                    "",
                    false,
                    identifier,
                    [&](libtorrent::error_code &error, libtorrent::torrent_handle &handle) {} );
    addNodeTimer.stop();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Node::registerMetaTypes();

    addNode(); // Add first node, let it announce its secondary hash and add another node after 15 seconds
    addNodeTimer.connect(&addNodeTimer, &QTimer::timeout, [&]() { addNode(); });
    addNodeTimer.start(15000); // 15 seconds from now

    // Print a list of JS nodes every 5 seconds
    printJSNodesTimer.connect(&printJSNodesTimer, &QTimer::timeout, [&]() { printJSNodes(); });
    printJSNodesTimer.start(5000);

    return a.exec();
}
