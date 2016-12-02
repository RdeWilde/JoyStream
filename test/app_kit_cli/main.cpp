/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <app_kit/kit.hpp>

#include <bitcoin/SPVWallet.hpp>
#include <common/P2PKHAddress.hpp>
#include <core/core.hpp>

#include <protocol_session/Exceptions.hpp>
#include <common/Network.hpp>

#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/asio/impl/src.hpp>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <ctime>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>

#include <iostream>
#include <string>

#include <cli.hpp>
#include <buyqueue.hpp>
#include <sellqueue.hpp>
#include <SignalHandler.hpp>

void claimRefunds(joystream::appkit::AppKit* kit, uint64_t txFee) {
    // Attempt to claim refunds
    for(auto refund : joystream::appkit::util::outputsToRefunds(kit->wallet()->getNonStandardStoreControlledOutputs())) {
        // suggested improvements - we should check that the locktime has expired
        //                        - combine multiple refunds into one transaction
        auto destination = kit->wallet()->generateReceiveAddress();
        auto tx = refund.getSignedSpendingTransaction(destination, txFee);
        kit->broadcastTransaction(tx);
        std::cout << "Broadcasting Refund Tx: " << Coin::TransactionId::fromTx(tx).toRPCByteOrder() << std::endl;
    }
}

std::string stateToString(libtorrent::torrent_status::state_t state) {
    switch (state) {
        case libtorrent::torrent_status::state_t::allocating: return "allocating";
        case libtorrent::torrent_status::state_t::checking_files: return "checking_files";
        case libtorrent::torrent_status::state_t::checking_resume_data: return "checking_resume_data";
        case libtorrent::torrent_status::state_t::downloading: return "downloading";
        case libtorrent::torrent_status::state_t::downloading_metadata: return "downloading_metadata";
        case libtorrent::torrent_status::state_t::finished: return "finished";
        case libtorrent::torrent_status::state_t::seeding: return "seeding";
    }

    throw std::runtime_error("invalid state");
}

int64_t sumOfOutputs(const std::vector<joystream::bitcoin::Store::StoreControlledOutput> &standardOutputs) {
    return std::accumulate(standardOutputs.begin(), standardOutputs.end(), (int64_t)0,
                           [](int64_t &sum, joystream::bitcoin::Store::StoreControlledOutput output) -> int64_t {
        return sum + output.value;
    });
}

void dumpWalletInfo(joystream::bitcoin::SPVWallet *wallet) {

    std::vector<Coin::P2PKHAddress> addresses = wallet->listReceiveAddresses();

    std::string depositAddress;

    if(addresses.size() > 0){
        depositAddress = addresses.front().toBase58CheckEncoding().toStdString();
    } else {
        depositAddress = wallet->generateReceiveAddress().toBase58CheckEncoding().toStdString();
    }

    std::cout << "Wallet Deposit Address: " <<  depositAddress << std::endl;

    std::cout << "Wallet Balance: " << wallet->unconfirmedBalance() << std::endl;

    auto outputChannels = joystream::appkit::util::outputsToOutboundPaymentChannelCommitments(wallet->getNonStandardStoreControlledOutputs());
    auto inputChannels = joystream::appkit::util::outputsToInboundPaymentChannelCommitments(wallet->getNonStandardStoreControlledOutputs());

    int64_t outgoingLockedFunds = std::accumulate(outputChannels.begin(), outputChannels.end(), (int64_t)0,
                                                  [](int64_t &sum, joystream::paymentchannel::Commitment &commitment) -> int64_t {
        return sum + commitment.value();
    });

    int64_t incomingLockedFunds = std::accumulate(inputChannels.begin(), inputChannels.end(), (int64_t)0,
                                                  [](int64_t &sum, joystream::paymentchannel::Commitment &commitment) -> int64_t {
        return sum + commitment.value();
    });

    std::cout << "Wallet funds locked in outbound payment channels: " << outgoingLockedFunds << std::endl;
    std::cout << "Wallet potential unsettled funds in inbound payment channels: " << incomingLockedFunds << std::endl;
    std::cout << "Wallet spendable P2PKH balance: " << sumOfOutputs(wallet->getStandardStoreControlledOutputs()) << std::endl;

}

class CliApp : public SignalHandler {

public:

    CliApp(int argc, char *argv[], const std::string &dataDir, Coin::Network network) :
        _app(argc, argv),
        _dataDir(dataDir),
        _network(network),
        _shuttingDown(false)
    {
        parseArgs();

        createAppKitInstance();

        QObject::connect(this, &SignalHandler::signalReceived, this, &CliApp::handleSignal);
    }

    ~CliApp() {
        delete _kit;
    }

    int run();

public slots:
    void handleSignal(int signal);

private:

    void parseArgs();
    void createAppKitInstance();
    void processTorrent();

    QCoreApplication _app;
    std::string _command;
    std::string _argument;
    joystream::appkit::Settings _appKitSettings;
    joystream::appkit::DataDirectory _dataDir;
    joystream::appkit::AppKit* _kit;
    const Coin::Network _network;
    bool _shuttingDown;

};

void CliApp::parseArgs()
{
    int argc = _app.arguments().size();

    if(argc > 1) _command = _app.arguments().at(1).toStdString();
    if(argc > 2) _argument = _app.arguments().at(2).toStdString();

    if(_command == "info"){
        _appKitSettings.autoStartWalletSync = false;
    }

    if((_command == "buy" || _command == "sell") && _argument == "") {
        throw std::runtime_error("Missing Torrent argument");
    }
}

void CliApp::createAppKitInstance()
{
    std::cout << "Creating AppKit Instance\n";
    _kit = joystream::appkit::AppKit::create(_dataDir.walletFilePath().toStdString(),
                                             _dataDir.blockTreeFilePath().toStdString(),
                                             _network,
                                             _appKitSettings);
}

void CliApp::handleSignal(int signal)
{
    if(signal & SignalHandler::SIGNALS::QUIT_SIGNALS) {
        //only handle shutdown signal once
        if(_shuttingDown)
            return;

        _shuttingDown = true;

        _kit->shutdown([this](){
            _app.quit();
        });
    }
}

void CliApp::processTorrent() {
    joystream::core::TorrentIdentifier* torrentIdentifier;

    try {
        if(_command == "buy" || _command == "sell") {

            assert(_argument != "");

            torrentIdentifier = joystream::appkit::util::makeTorrentIdentifier(_argument);

            if(torrentIdentifier == nullptr) {
                std::cout << "Warning: Invalid torrent argument" << std::endl;
                return;
            }
        } else {
            return;
        }
    } catch(std::exception &e) {
        std::cout << "Error: Parsing torrent argument" << e.what() << std::endl;
        return;
    }

    if(torrentIdentifier) {
        std::cout << "Torrent InfoHash: " << torrentIdentifier->infoHash() << std::endl;

        auto torrent = _kit->addTorrent(*torrentIdentifier, _dataDir.defaultSavePath().toStdString());

        delete torrentIdentifier;

        joystream::protocol_wire::BuyerTerms buyerTerms(100, 5, 1, 20000);
        joystream::protocol_session::BuyingPolicy buyingPolicy(3, 25, joystream::protocol_wire::SellerTerms::OrderingPolicy::min_price);

        // If the add operation completed we can try to buy or sell
        if(torrent->isFinished()) {

            if(_command == "buy" && torrent->wasAdded())
                _kit->buyTorrent(torrent->infoHash(), buyingPolicy, buyerTerms);

        } else {
            // Wait for torrent to be added
            QObject::connect(torrent.get(), &joystream::appkit::TorrentAddResponse::finished, [this, torrent, buyerTerms, buyingPolicy](){
                if(_command == "buy" && torrent->wasAdded())
                    _kit->buyTorrent(torrent->infoHash(), buyingPolicy, buyerTerms);
            });
        }
    }
}

int CliApp::run()
{
    if(_shuttingDown)
        return 0;

    dumpWalletInfo(_kit->wallet());

    if(_command == "info")
        return 0;

    processTorrent();

    claimRefunds(_kit, 5000);

    std::cout << "Starting Qt Application Event loop\n";
    int ret = _app.exec();

    std::cout << "Exited Qt Application event loop with code: " << ret << std::endl;
    return ret;
}

int main(int argc, char *argv[])
{
    if(getenv("DEBUG") != NULL) {
        INIT_LOGGER("netsync.log");
    }

    auto env_data_dir = getenv("JOYSTREAM_DATADIR");
    std::string kitDataDirectory =  env_data_dir != NULL ? env_data_dir : QDir::currentPath().toStdString();

    CliApp app(argc, argv, kitDataDirectory, Coin::Network::testnet3);
    int ret = app.run();
    return ret;
}
