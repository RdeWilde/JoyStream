/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 2016
 */

#ifndef JOYSTREAM_APPKIT_HPP
#define JOYSTREAM_APPKIT_HPP

#include <functional>
#include <QString>
#include <QTimer>

#include <app_kit/TransactionSendBuffer.hpp>
#include <app_kit/Settings.hpp>
#include <app_kit/WorkerResult.hpp>

#include <bitcoin/SPVWallet.hpp>

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

class AppKit : public QObject
{
    Q_OBJECT

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
    std::shared_ptr<WorkerResult> addTorrent(const joystream::appkit::SavedTorrentParameters&);

    std::shared_ptr<WorkerResult> addTorrent(const core::TorrentIdentifier&, const std::string& savePath);

    std::shared_ptr<WorkerResult> buyTorrent(libtorrent::sha1_hash, const protocol_session::BuyingPolicy& policy, const protocol_wire::BuyerTerms& terms);

    std::shared_ptr<WorkerResult> sellTorrent(libtorrent::sha1_hash, const protocol_session::SellingPolicy& policy, const protocol_wire::SellerTerms& terms);

    void broadcastTransaction(Coin::Transaction &) const;

private:

    // Do not allow copying
    AppKit(const AppKit &);
    AppKit& operator=(const AppKit&);

    AppKit(const Settings&,
           std::unique_ptr<bitcoin::SPVWallet>&,
           std::unique_ptr<TransactionSendBuffer>&,
           std::unique_ptr<core::Node>&);

    Settings _settings;

    bool _trySyncWallet;

    std::unique_ptr<bitcoin::SPVWallet> _wallet;

    std::unique_ptr<TransactionSendBuffer> _transactionSendBuffer;

    std::unique_ptr<core::Node> _node;

    QTimer _timer;

    protocol_session::GenerateP2SHKeyPairCallbackHandler paychanKeysGenerator();
    Coin::KeyPair paychanKeysGeneratorFunction(const protocol_session::P2SHScriptGeneratorFromPubKey&, const uchar_vector&);

    protocol_session::GenerateReceiveAddressesCallbackHandler receiveAddressesGenerator();
    std::vector<Coin::P2PKHAddress> receiveAddressesGeneratorFunction(int npairs);

    protocol_session::GenerateChangeAddressesCallbackHandler changeAddressesGenerator();
    std::vector<Coin::P2PKHAddress> changeAddressesGeneratorFunction(int npairs);

};

}
}
#endif // JOYSTREAM_APPKIT_HPP
