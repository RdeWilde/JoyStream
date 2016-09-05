#ifndef JOYSTREAM_APP_KIT_HPP
#define JOYSTREAM_APP_KIT_HPP

#include <functional>
#include <QString>

//#include <iostream>

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

    static AppKit* createInstance(const QString &dataDirectory, Coin::Network network);

    std::unique_ptr<bitcoin::SPVWallet> & wallet();
    std::unique_ptr<core::Node> & node();

    void syncWallet(std::string host, int port);

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

    static bitcoin::SPVWallet* getWallet(const QString &dataDirectory, Coin::Network network);

    AppKit();

    AppKit(std::unique_ptr<core::Node> &node, std::unique_ptr<bitcoin::SPVWallet> &wallet, const QString &dataDirectory);

    QString _dataDirectory;

    std::unique_ptr<core::Node> _node;

    std::unique_ptr<bitcoin::SPVWallet> _wallet;
};

}
#endif // JOYSTREAM_APP_KIT_HPP
