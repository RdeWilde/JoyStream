#ifndef JOYSTREAM_APPKIT_HPP
#define JOYSTREAM_APPKIT_HPP

#include <core/Node.hpp>
#include <bitcoin/SPVWallet.hpp>

#include <iostream>

#include <QDir>

namespace joystream {

class AppKit
{

public:
    AppKit();

    std::shared_ptr<joystream::core::Node> createAndStartNode();

    // Save Node state to and ostream
    //void saveNodeState(ostream&);

    // Load Node state from an istream
    //void loadNodeState(istream&);

    // Save Node state to torrent data directory
    void saveNodeState();

    // Load Node state from torrent data directory
    void loadNodeState();

    void registerTransactionBroadcaster(const joystream::core::BroadcastTransaction & broadcastTransaction);
    void registerWalletAsTransactionBroadcaster();
    void stopBroadcastingTransactions();

    void broadcastTx(const Coin::Transaction &);


    //void buyTorrent(libtorrent::sha1_hash &infohash);
    void buyTorrent(std::shared_ptr<joystream::core::Torrent> &,
                    protocol_session::BuyingPolicy&,
                    protocol_wire::BuyerTerms&,
                    extension::request::SubroutineHandler &);

private:
    // Location of wallet data directory
    QDir walletDataDir_;

    // Location of Torrent data directory
    QDir torrentDataDir_;

    std::shared_ptr<joystream::core::Node> _node;

    joystream::core::BroadcastTransaction _registeredTransactionBroadcaster;

    std::shared_ptr<joystream::bitcoin::SPVWallet> _wallet;
};

}
#endif // JOYSTREAM_APPKIT_HPP
