#ifndef JOYSTREAM_APPKIT_HPP
#define JOYSTREAM_APPKIT_HPP

#include <functional>
#include <QString>
#include <QTimer>
#include <QLockFile>

#include <app_kit/DataDirectory.hpp>
#include <app_kit/TransactionSendQueue.hpp>
#include <app_kit/Settings.hpp>

#include <core/core.hpp>

#include <iostream>

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
class SavedTorrents;
class SavedTorrentParameters;

class AppKit
{

public:
    typedef std::function<void()> Callback;
    typedef std::function<void(const std::exception_ptr &)> SubroutineHandler;

    static AppKit* create(std::string dataDirectoryPath, Coin::Network, const Settings&);

    bitcoin::SPVWallet* wallet();
    core::Node* node();

    void shutdown(const Callback &);

    void applySettings(const Settings&);

    SavedTorrents generateSavedTorrents() const;

    SavedTorrents loadSavedTorrents() const;

    // Generate node data
    QJsonObject generateNodeData() const;

    // Save torrents managed by the Node to disk
    void saveNodeData() const;

    // Read saved torrents from disk - does not change the current Node State
    QJsonObject loadNodeData() const;

    // Add torrent from TorrentState
    void addTorrent(const joystream::appkit::SavedTorrentParameters &torrent, const core::Node::AddedTorrent &);

    // Add torrent with default parameters
    void addTorrent(const core::TorrentIdentifier &, const core::Node::AddedTorrent &);

    void buyTorrent(const core::Torrent*,
                    const protocol_session::BuyingPolicy &,
                    const protocol_wire::BuyerTerms &,
                    const SubroutineHandler &);

    void sellTorrent(const core::Torrent *,
                     const protocol_session::SellingPolicy &,
                     const protocol_wire::SellerTerms &,
                     const SubroutineHandler &);

private:

    static bitcoin::SPVWallet* getWallet(const DataDirectory &dataDirectory, Coin::Network network);

    AppKit(core::Node *node, bitcoin::SPVWallet *wallet, TransactionSendQueue *txSendQueue, DataDirectory *dataDirectory, const Settings &settings);

    void syncWallet();

    std::unique_ptr<core::Node> _node;

    std::unique_ptr<bitcoin::SPVWallet> _wallet;

    std::unique_ptr<DataDirectory> _dataDirectory;

    Settings _settings;

    QTimer *_timer;

    std::unique_ptr<TransactionSendQueue> _transactionSendQueue;

    bool _shuttingDown;

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
