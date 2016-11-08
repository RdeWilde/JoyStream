#ifndef JOYSTREAM_APPKIT_HPP
#define JOYSTREAM_APPKIT_HPP

#include <functional>
#include <QString>
#include <QTimer>
#include <QLockFile>

#include <app_kit/DataDirectory.hpp>
#include <app_kit/TransactionSendBuffer.hpp>
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

    static AppKit* create(const std::string &walletFilePath,
                          const std::string &walletBlockTreeFilePath,
                          Coin::Network, const Settings &settings = Settings());

    bitcoin::SPVWallet* wallet();
    core::Node* node();

    void syncWallet();

    void shutdown(const Callback &);

    void applySettings(const Settings&);

    SavedTorrents generateSavedTorrents() const;

    // Add torrent from TorrentState
    void addTorrent(const joystream::appkit::SavedTorrentParameters &torrent, const core::Node::AddedTorrent &);

    void buyTorrent(const core::Torrent*,
                    const protocol_session::BuyingPolicy &,
                    const protocol_wire::BuyerTerms &,
                    const SubroutineHandler &);

    void sellTorrent(const core::Torrent *,
                     const protocol_session::SellingPolicy &,
                     const protocol_wire::SellerTerms &,
                     const SubroutineHandler &);

    int64_t getStandardWalletBalance(int confirmations = 0) const;
    std::vector<paymentchannel::Commitment> getOutboundPaymentChannelCommitments(int confirmations = 0) const;
    std::vector<paymentchannel::Commitment> getInboundPaymentChannelCommitments(int confirmations = 0) const;
    std::vector<paymentchannel::Refund> getRefunds(int confirmations = 0) const;

    void broadcastTransaction(Coin::Transaction &) const;

private:

    static bitcoin::SPVWallet* getWallet(const std::string &storeFile, const std::string blockTreeFile, Coin::Network network);

    AppKit(core::Node *node, bitcoin::SPVWallet *wallet, TransactionSendBuffer*, const Settings &settings);

    std::unique_ptr<core::Node> _node;

    std::unique_ptr<bitcoin::SPVWallet> _wallet;

    QTimer _timer;

    std::unique_ptr<TransactionSendBuffer> _transactionSendBuffer;

    Settings _settings;

    bool _trySyncWallet;

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
