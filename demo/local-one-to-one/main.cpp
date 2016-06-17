/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 23 2015
 */

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QNetworkAccessManager>

#include <runner/Runner.hpp>
#include <runner/ControllerBarrier.hpp>
#include <common/Seed.hpp>

#include <boost/asio/impl/src.hpp>

/**
 * Demo meant to run two clients, one seller and one buyer,
 * on tracker - most likely local, since most public
 * trackers get confused with two clients from same IP.
 *
 * HOW TO RUN
 * ==============================================================
 *
 * This application requires two arguments
 *
 * a) the directory (with no trailing slash) where torrent file are found
 *    NB!!: if loaded files are local torrents, remember to start tracker!
 *
 * b) the directory (with no trailing slash) where torrent data is found
 *
 * e.g: ./local-one-to-one /home/bedeho/JoyStream/Development/JoyStream/torrents/localhost /home/bedeho/JoyStream/Data
 */

// Entry point
int main(int argc, char* argv[]) {

    // Create Qt application: all objects created after this point are owned by this thread
    QApplication app(argc, argv);
    QApplication::setApplicationName(LOCALONETOONE_APPLICATION_NAME);

    // Setup command line parsing
    QCommandLineParser parser;
    parser.setApplicationDescription(LOCALONETOONE_APPLICATION_DESCRIPTION);

    // Add options
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("Torrent file directory", "Directory which contains torrent files needed.");
    parser.addPositionalArgument("Torrent data directory", "Directory which data for torrent files needed.");

    // Process the actual command line arguments given by the user
    parser.process(app);

    // Read positional arguments
    QStringList arguments = parser.positionalArguments();

    if(arguments.size() != 2) {
        std::cerr << "Exactly two arguments must be provided.";
        return 1;
    }

    // Get first argument: Torrent file directory
    QString torrentFileDirectory = arguments.first();

    if(!QDir(torrentFileDirectory).exists()) {
        std::cerr << torrentFileDirectory.toStdString() << " directory does not exist.";
        return 1;
    }

    // Derive torrent file paths
    QString RISE_AND_RISE_OF_BITCOIN_FILE   = torrentFileDirectory + QDir::separator() + "The.Rise.and.Rise.of.Bitcoin.2014.720p.HDrip.x264.AAC.MVGroup.org.mp4.torrent",
            LITTLE_SIMZ_FILE                = torrentFileDirectory + QDir::separator() + "Little Simz - AGE 101- DROP 1 - 01 Homosapiens VS Aliens.mp3.torrent";

    QString TORRENT = RISE_AND_RISE_OF_BITCOIN_FILE;

    // Get second argument: Torrent data directory
    QString torrentDataDirectory = arguments[1];

    if(!QDir(torrentDataDirectory).exists()) {
        std::cerr << torrentDataDirectory.toStdString() << " directory does not exist.";
        return 1;
    }

    // Location where test clients create their home folders,
    // i.e. where they load/save actual wallet/torrent data
    QString homeDirectory = QDir::currentPath();

    // Network access manager
    QNetworkAccessManager manager;

    // Create a controller tracker
    Runner::ControllerBarrier barrier;

    // Load default configurations for the controller
    Controller::Configuration configuration;

    // BUYER
    // ***********************************************************
    Controller * buyer = Runner::create_client(configuration,
                                                true,
                                                false,
                                                homeDirectory,
                                                QString("lone_buyer"),
                                                Coin::Seed("12222222245702754273542035678056807968507856907805687056875807896570586786507890586075806786709586078567095867085607865087065986"),
                                                LOCALONETOONE_BITCOIN_NETWORK,
                                                &manager,
                                                LOCALONETOONE_BLOCKCYPHER_TOKEN);

    BuyerTorrentPlugin::Configuration buyerPluginConfiguration(false,
                                                               5,         // Maximum piece price (satoshi)
                                                               4*3600,    // Maximum lock time on refund (seconds)
                                                               10000,     // BitCoinRepresentation NOT WORKING FOR WHATEVER REASON: BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.1).satoshies(), // Max fee per kB (satoshi)
                                                               1          // #sellers
                                                               );

    Controller::Torrent::Configuration buyerTorrentConfiguration = Runner::create_torrent_configuration(TORRENT, homeDirectory + QDir::separator() + "lone_buyer" + QDir::separator() + "torrent_data");

    try {

        Coin::UnspentP2PKHOutput utxo = Runner::find_utxo((buyerTorrentConfiguration.torrentFile())->num_pieces(),
                                                          buyerPluginConfiguration,
                                                          buyer->wallet());

        buyer->addTorrent(buyerTorrentConfiguration, buyerPluginConfiguration, utxo);

    } catch (const std::runtime_error & e) {
        qDebug() << "Torrent not added to buyer.";
    }

    barrier.add(buyer);

    // SELLER
    // ***********************************************************
    Controller * seller = Runner::create_client(configuration,
                                                true,
                                                false,
                                                homeDirectory,
                                                QString("lone_seller"),
                                                Coin::Seed("16984699575934757385634735783465834785347589734857345834975893475934985390128038291038120983091839081203812098381209981381248931"),
                                                LOCALONETOONE_BITCOIN_NETWORK,
                                                &manager,
                                                LOCALONETOONE_BLOCKCYPHER_TOKEN);

    SellerTorrentPlugin::Configuration sellerPluginConfiguration(false,
                                                                 2,       // Minimum piece price (satoshi)
                                                                 2*3600,   // Minimum lock time on refund (seconds)
                                                                 5000,     // BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.01).satoshies(), // Min fee per kB (satoshi)
                                                                 1,        // Max #seller
                                                                 17*60    // Maximum contract confirmation delay (seconds)
                                                                 );

    Controller::Torrent::Configuration sellerTorrentConfiguration = Runner::create_torrent_configuration(TORRENT, torrentDataDirectory);

    seller->addTorrent(sellerTorrentConfiguration, sellerPluginConfiguration);

    barrier.add(seller);

    // Wait for both to end
    barrier.erect();

    return 0;
}

/**

const QVector<Coin::Seed> sellerTestSeeds = {
    Coin::Seed("22222222245702754273542035678056807968507856907805687056875807896570586786507890586075806786709586078567095867085607865087065986"),
    Coin::Seed("43333333563563567232345642423047590475094237954237873052437527309857043275903750273489573402957023475807340573290570943010124893"),
    Coin::Seed("18835555555555555559192803809218098290382093810928390182390821093809218390128038291038120983091839081203812098381209981381248931"),
    Coin::Seed("87356356644444444444444444457463576345687346584367856437658436586348586436578436876836856348685683468376856384567346536867488931")
};

const QVector<Coin::Seed> buyerTestSeeds = {
    Coin::Seed("06780569785690879056879056840569895895479376954967590786805698087905867085690785690785690789056870956567095867085607865087065986"),
    Coin::Seed("23414235616345624365416234564615635342343465347632564535673465736576837465834750273489573402957023475807340573290570943010124893"),
    Coin::Seed("56984699575934757385634735783465834785347589734857345834975893475934985390128038291038120983091839081203812098381209981381248931"),
    Coin::Seed("08908089890805687908657567065808305834905834985038453840583485083953453475673846538564685384756384583686856384567346536867488931")
};

const QVector<SellerTorrentPlugin::Configuration> sellerConfigurations = {
    SellerTorrentPlugin::Configuration(false,
                                      2,       // Minimum piece price (satoshi)
                                      2*3600,   // Minimum lock time on refund (seconds)
                                      5000,     // BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.01).satoshies(), // Min fee per kB (satoshi)
                                      1,        // Max #seller
                                      17*60)    // Maximum contract confirmation delay (seconds)
};

const QVector<BuyerTorrentPlugin::Configuration> buyerConfigurations = {
    BuyerTorrentPlugin::Configuration(false,
                                     5,         // Maximum piece price (satoshi)
                                     4*3600,     // Maximum lock time on refund (seconds)
                                     10000,      // BitCoinRepresentation NOT WORKING FOR WHATEVER REASON: BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.1).satoshies(), // Max fee per kB (satoshi)
                                     1)          // #sellers
};

// Load torrent
libtorrent::torrent_info torrentInfo = Runner::load_torrent(RISE_AND_RISE_OF_BITCOIN_FILE.c_str());

// Sellers
Controller::Torrent::Configuration torrentConfiguration = Runner::create_torrent_configuration(torrentInfo, QString::fromStdString(torrentsDirectory));

Runner::add_sellers_with_plugin(configuration,
                                barrier,
                                true,
                                true,
                                homeDirectory,
                                torrentConfiguration,
                                sellerConfigurations,
                                sellerTestSeeds,
                                LOCALONETOONE_BITCOIN_NETWORK,
                                &manager,
                                LOCALONETOONE_BLOCKCYPHER_TOKEN);
// Buyers
Runner::add_buyers_with_plugin(configuration,
                               barrier,
                               true,
                               true,
                               homeDirectory,
                               torrentInfo,
                               buyerConfigurations,
                               buyerTestSeeds,
                               LOCALONETOONE_BITCOIN_NETWORK,
                               &manager,
                               LOCALONETOONE_BLOCKCYPHER_TOKEN);

*/
