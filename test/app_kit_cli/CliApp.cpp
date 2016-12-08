#include "CliApp.hpp"

#include <common/P2PKHAddress.hpp>
#include <protocol_session/Exceptions.hpp>

#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QFile>

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/asio.hpp>

int64_t sumOfOutputs(const std::vector<joystream::bitcoin::Store::StoreControlledOutput> &standardOutputs) {
    return std::accumulate(standardOutputs.begin(), standardOutputs.end(), (int64_t)0,
                           [](int64_t &sum, joystream::bitcoin::Store::StoreControlledOutput output) -> int64_t {
        return sum + output.value;
    });
}

CliApp::CliApp(int argc, char *argv[], const std::string &dataDir, Coin::Network network) :
    _app(argc, argv),
    _dataDir(dataDir),
    _network(network),
    _shuttingDown(false)
{
    parseArgs();

    std::cout << "Creating AppKit Instance\n";
    _kit.reset(joystream::appkit::AppKit::create(_dataDir.walletFilePath().toStdString(),
                                                 _dataDir.blockTreeFilePath().toStdString(),
                                                 _network,
                                                 _appKitSettings));

    QObject::connect(this, &SignalHandler::signalReceived, this, &CliApp::handleSignal);
}

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

void CliApp::handleSignal(int signal)
{
    if(signal & SignalHandler::SIGNALS::QUIT_SIGNALS) {
        //only handle shutdown signal once
        if(_shuttingDown)
            return;

        _shuttingDown = true;

        joystream::appkit::SavedTorrents torrents = _kit->generateSavedTorrents();

        auto savePath = _dataDir.savedTorrentsFilePath();

        try {
            saveTorrentsToFile(torrents, savePath);
        } catch(std::exception &e) {
            std::cout << "Error saving torrents:";
            std::cout << e.what() << std::endl;
        }

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

        // Wait for torrent to be added
        QObject::connect(torrent.get(), &joystream::appkit::WorkerResult::finished, [this, torrent](){
            if(torrent->getError() != joystream::appkit::WorkerResult::Error::NoError)
                return;

            if(_command == "buy") {
                joystream::protocol_wire::BuyerTerms buyerTerms(100, 5, 1, 20000);
                joystream::protocol_session::BuyingPolicy buyingPolicy(3, 25, joystream::protocol_wire::SellerTerms::OrderingPolicy::min_price);

                auto buyer = _kit->buyTorrent(torrent->infoHash(), buyingPolicy, buyerTerms);

                QObject::connect(buyer.get(), &joystream::appkit::WorkerResult::finished, [this, buyer](){
                   std::cout << "Buyer Finished:" << std::endl;
                });

            } else if(_command == "sell") {
                joystream::protocol_wire::SellerTerms sellerTerms(100, 5, 1, 20000, 5000);
                joystream::protocol_session::SellingPolicy sellingPolicy;

                auto seller = _kit->sellTorrent(torrent->infoHash(), sellingPolicy, sellerTerms);

                QObject::connect(seller.get(), &joystream::appkit::WorkerResult::finished, [this, seller](){
                   std::cout << "Seller Finished:" << std::endl;
                });
            }
        });

    }
}

int CliApp::run()
{
    if(_shuttingDown)
        return 0;

    dumpWalletInfo();

    if(_command == "info")
        return 0;

    processTorrent();

    claimRefunds(5000);

    try {
        loadTorrents();
    } catch (std::exception &e) {
        std::cout << "Error Loading Torrents: ";
        std::cout << e.what() << std::endl;
    }

    std::cout << "Starting Qt Application Event loop\n";
    int ret = _app.exec();

    std::cout << "Exited Qt Application event loop with code: " << ret << std::endl;
    return ret;
}

void CliApp::claimRefunds(uint64_t txFee) {
    // Attempt to claim refunds
    for(auto refund : joystream::appkit::util::outputsToRefunds(_kit->wallet()->getNonStandardStoreControlledOutputs())) {
        // suggested improvements - we should check that the locktime has expired
        //                        - combine multiple refunds into one transaction
        auto destination = _kit->wallet()->generateReceiveAddress();
        auto tx = refund.getSignedSpendingTransaction(destination, txFee);
        _kit->broadcastTransaction(tx);
        std::cout << "Broadcasting Refund Tx: " << Coin::TransactionId::fromTx(tx).toRPCByteOrder() << std::endl;
    }
}

void CliApp::dumpWalletInfo() {

    auto wallet = _kit->wallet();

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

void CliApp::saveTorrentsToFile(joystream::appkit::SavedTorrents savedTorrents, QString savePath)
{
    auto data = serializeSavedTorrents(savedTorrents);

    writeDataToFile(savePath, data);
}

QByteArray CliApp::serializeSavedTorrents(joystream::appkit::SavedTorrents savedTorrents) {
    QJsonObject rootObj;

    rootObj["torrents"] = savedTorrents.toJson();

    QJsonDocument doc(rootObj);

    return doc.toJson();
}

void CliApp::writeDataToFile(QString filePath, QByteArray data) {
    QFile saveFile(filePath);

    saveFile.open(QIODevice::WriteOnly);

    saveFile.write(data);

    saveFile.close();
}

QByteArray CliApp::readDataFromFile(QString filePath) {
    QFile saveFile(filePath);

    saveFile.open(QIODevice::ReadOnly);

    auto data = saveFile.readAll();

    saveFile.close();

    return data;
}

joystream::appkit::SavedTorrents CliApp::deserializeSavedTorrents(QByteArray data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);

    auto savedTorrents = doc.object()["torrents"];

    return joystream::appkit::SavedTorrents(savedTorrents);
}

void CliApp::loadTorrents()
{
    auto savePath = _dataDir.savedTorrentsFilePath();

    if(!QFile::exists(savePath))
        return;

    auto serializedSavedTorrents = readDataFromFile(savePath);

    auto savedTorrents = deserializeSavedTorrents(serializedSavedTorrents);

    for(const auto &torrent : savedTorrents.torrents()) {
        joystream::appkit::SavedTorrentParameters torrentParams = torrent.second;
        joystream::appkit::SavedSessionParameters sessionParams = torrentParams.sessionParameters();

        auto addResult = _kit->addTorrent(torrentParams);

        QObject::connect(addResult.get(), &joystream::appkit::WorkerResult::finished, [this, sessionParams, addResult]() {

            if(addResult->getError() != joystream::appkit::WorkerResult::Error::NoError)
                return;

            if(sessionParams.mode() == joystream::protocol_session::SessionMode::buying) {
                _kit->buyTorrent(addResult->infoHash(), sessionParams.buyingPolicy(), sessionParams.buyerTerms());

            } else if(sessionParams.mode() == joystream::protocol_session::SessionMode::selling) {

                _kit->sellTorrent(addResult->infoHash(), sessionParams.sellingPolicy(), sessionParams.sellerTerms());
            }
        });
    }
}
