#ifndef JOYSTREAM_APPKIT_HPP
#define JOYSTREAM_APPKIT_HPP

#include <functional>
#include <QString>
#include <QTimer>
#include <QLockFile>

#include <app_kit/DataDirectory.hpp>
#include <core/core.hpp>

//#include <iostream>

namespace libtorrent {
    class sha1_hash;
}

namespace Coin {
    class Transaction;
    enum class Network;
}

namespace joystream {

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

namespace appkit {

struct Settings;

class AppKit
{

public:
    typedef std::function<void()> Callback;
    typedef std::function<void(const std::exception_ptr &)> SubroutineHandler;

    static AppKit* create(const Settings&);

    bitcoin::SPVWallet* wallet();
    core::Node* node();

    void shutdown(const Callback &);

    // Save Node state to and ostream
    //void saveNodeState(ostream&);

    // Load Node state from an istream
    //void loadNodeState(istream&);

    // Save Node state to torrent data directory
    void saveNodeState();

    // Load Node state from torrent data directory
    void loadNodeState();

    void addTorrent(const core::TorrentIdentifier &, const core::Node::AddedTorrent &);

    void buyTorrent(const core::Torrent*,
                    const protocol_session::BuyingPolicy &,
                    const protocol_wire::BuyerTerms &,
                    const SubroutineHandler &);

    void sellTorrent(const core::Torrent *,
                     const protocol_session::SellingPolicy &,
                     const protocol_wire::SellerTerms &,
                     const SubroutineHandler &);

    // Utility functions
    static libtorrent::sha1_hash sha1_hash_from_hex_string(const char *);
    static core::TorrentIdentifier* makeTorrentIdentifier(const char *);

private:

    static bitcoin::SPVWallet* getWallet(const DataDirectory &dataDirectory, Coin::Network network);

    AppKit(core::Node *node, bitcoin::SPVWallet *wallet, DataDirectory *dataDirectory, std::string host, int port);

    void syncWallet();

    std::unique_ptr<core::Node> _node;

    std::unique_ptr<bitcoin::SPVWallet> _wallet;

    std::unique_ptr<DataDirectory> _dataDirectory;

    std::string _bitcoinHost;
    int _bitcoinPort;

    QTimer *_timer;

    void buyTorrent(core::TorrentPlugin *,
                    const protocol_session::BuyingPolicy &,
                    const protocol_wire::BuyerTerms &,
                    const SubroutineHandler &, Coin::UnspentOutputSet outputs);

    void sellTorrent(core::TorrentPlugin *,
                     const protocol_session::SellingPolicy &,
                     const protocol_wire::SellerTerms &,
                     const SubroutineHandler &);

    uint64_t estimateRequiredFundsToBuyTorrent(const core::Torrent *, joystream::protocol_wire::BuyerTerms);
};

}
}
#endif // JOYSTREAM_APPKIT_HPP
