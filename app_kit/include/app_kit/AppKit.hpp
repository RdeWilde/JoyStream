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
    class TorrentPlugin;
}

namespace bitcoin {
    class SPVWallet;
}

namespace protocol_session {
    class BuyingPolicy;
    class SellingPolicy;
}

namespace protocol_wire {
    class BuyerTerms;
    class SellerTerms;
}

class AppKit
{

public:
    typedef std::function<void()> Callback;
    //typedef std::function<void(const Coin::Transaction &)> BroadcastTransaction;
    typedef std::function<void(const std::exception_ptr &)> SubroutineHandler;

    static AppKit* createInstance(const QString &dataDirectory, Coin::Network network);

    bitcoin::SPVWallet* wallet();
    core::Node* node();

    void syncWallet(std::string host = "", int port = 0);

    void shutdown(const Callback &);

    // Save Node state to and ostream
    //void saveNodeState(ostream&);

    // Load Node state from an istream
    //void loadNodeState(istream&);

    // Save Node state to torrent data directory
    void saveNodeState();

    // Load Node state from torrent data directory
    void loadNodeState();

    void buyTorrent(core::TorrentPlugin *,
                    const protocol_session::BuyingPolicy &,
                    const protocol_wire::BuyerTerms &,
                    const SubroutineHandler &);

    void buyTorrent(const core::Torrent*,
                    const protocol_session::BuyingPolicy &,
                    const protocol_wire::BuyerTerms &,
                    const SubroutineHandler &);

    void buyTorrent(const libtorrent::sha1_hash &info_hash,
                    const protocol_session::BuyingPolicy&,
                    const protocol_wire::BuyerTerms&,
                    const SubroutineHandler &);

    void sellTorrent(const core::Torrent *,
                     const protocol_session::SellingPolicy &,
                     const protocol_wire::SellerTerms &,
                     const SubroutineHandler &);

    void sellTorrent(core::TorrentPlugin *,
                     const protocol_session::SellingPolicy &,
                     const protocol_wire::SellerTerms &,
                     const SubroutineHandler &);
private:

    static bitcoin::SPVWallet* getWallet(const QString &dataDirectory, Coin::Network network);

    AppKit();

    AppKit(core::Node *node, bitcoin::SPVWallet *wallet, const QString &dataDirectory);

    QString _dataDirectory;

    std::unique_ptr<core::Node> _node;

    std::unique_ptr<bitcoin::SPVWallet> _wallet;
};

}
#endif // JOYSTREAM_APP_KIT_HPP
