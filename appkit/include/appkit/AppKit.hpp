#ifndef JOYSTREAM_APPKIT_HPP
#define JOYSTREAM_APPKIT_HPP

#include <functional>

//#include <iostream>

//#include <QDir>

namespace libtorrent {
    class sha1_hash;
}

namespace Coin {
    class Transaction;
    enum class Network;
}

namespace joystream {

namespace core {
    class Node;
    class Torrent;
}

namespace bitcoin {
    class SPVWallet;
}

namespace protocol_session {
    class BuyingPolicy;
}

namespace protocol_wire {
    class BuyerTerms;
}

class AppKit
{

public:
    typedef std::function<void(const Coin::Transaction &)> BroadcastTransaction;
    typedef std::function<void(const std::exception_ptr &)> SubroutineHandler;

    static bitcoin::SPVWallet* getWallet(std::string dataDirectory, Coin::Network network);

    static void createWallet(std::string dataDirectory, Coin::Network network);

    static AppKit* createInstance(std::string dataDirectory, Coin::Network network);

    std::unique_ptr<bitcoin::SPVWallet> & wallet();
    std::unique_ptr<core::Node> & node();

    // Save Node state to and ostream
    //void saveNodeState(ostream&);

    // Load Node state from an istream
    //void loadNodeState(istream&);

    // Save Node state to torrent data directory
    void saveNodeState();

    // Load Node state from torrent data directory
    void loadNodeState();

    void buyTorrent(std::shared_ptr<core::Torrent> &,
                    protocol_session::BuyingPolicy&,
                    protocol_wire::BuyerTerms&,
                    SubroutineHandler &);

    void buyTorrent(libtorrent::sha1_hash &info_hash,
                    protocol_session::BuyingPolicy&,
                    protocol_wire::BuyerTerms&,
                    SubroutineHandler &);
private:

    AppKit();

    AppKit(std::unique_ptr<core::Node> &node, std::unique_ptr<bitcoin::SPVWallet> &wallet);

    // Location of wallet data directory
    //QDir walletDataDir_;

    // Location of Torrent data directory
    //QDir torrentDataDir_;

    std::unique_ptr<core::Node> _node;

    std::unique_ptr<bitcoin::SPVWallet> _wallet;
};

}
#endif // JOYSTREAM_APPKIT_HPP
