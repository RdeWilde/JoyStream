/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 26 2015
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
 * Demo meant to be run against itself on public tracker,
 * can run in to roles (first and second).
 *
 * HOW TO RUN
 * ==============================================================
 *
 * This application requires at least two, and at most three, arguments
 *
 * a) the directory (with no trailing slash) where torrent file are found
 *    NB!!: if loaded files are local torrents, remember to start tracker!
 *
 * b) the directory (with no trailing slash) where torrent data is found
 *
 * c) -f if you want to run in first role, if not supplied, you run in second role
 *
 * e.g: ./local-one-to-one /home/bedeho/JoyStream/Development/JoyStream/torrents/public /home/bedeho/JoyStream/Data -f
 */

void buy(Controller * controller, const QString & savePath, const QString & torrentFile);
void sell(Controller * controller, const QString & torrentDataDirectory, const QString & torrentFile);
QString cleanup_savepath(const QString & homeDirectory, const QString & name);

// Entry point
int main(int argc, char* argv[]) {

    // Create Qt application: all objects created after this point are owned by this thread
    QApplication app(argc, argv);
    QApplication::setApplicationName(PUBLICONESYMMETRIC_APPLICATION_NAME);

    // Setup command line parsing
    QCommandLineParser parser;
    parser.setApplicationDescription(PUBLICONESYMMETRIC_APPLICATION_DESCRIPTION);

    // Add options
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("Torrent file directory", "Directory which contains torrent files needed.");
    parser.addPositionalArgument("Torrent data directory", "Directory which data for torrent files needed.");

    QCommandLineOption beFirstRole("f", "Be first role in symmetric scenario.");
    parser.addOption(beFirstRole);

    // Process the actual command line arguments given by the user
    parser.process(app);

    // Read positional arguments
    QStringList arguments = parser.positionalArguments();

    if(arguments.size() < 2) {
        std::cerr << "At least two arguments must be provided.";
        return 1;
    }

    // Get first argument: Torrent file directory
    QString torrentFileDirectory = arguments.first();

    if(!QDir(torrentFileDirectory).exists()) {
        std::cerr << torrentFileDirectory.toStdString() << " directory does not exist.";
        return 1;
    }

    // Derive torrent file paths
    QString p = torrentFileDirectory + QDir::separator();
    QString APOLLO_FILE                 = p + "apollo.torrent",
            HUBBLECAST_FILE             = p + "hubblecast.torrent",
            NASA_FILE                   = p + "Nasa.torrent",
            OPENSUSEISO_FILE            = p + "OpenSUSEiso.torrent",
            SFC_FILE                    = p + "sfc.torrent",
            STOCKFOOTAGECHRISTMAS_FILE  = p + "stock footage christmas.torrent",
            USNOW_FILE                  = p + "us_now.torrent";

    // Get second argument: Torrent data directory
    QString torrentDataDirectory = arguments[1];

    if(!QDir(torrentDataDirectory).exists()) {
        std::cerr << torrentDataDirectory.toStdString() << " directory does not exist.";
        return 1;
    }

    // Network access manager
    QNetworkAccessManager manager;

    // Create a controller tracker
    Runner::ControllerBarrier barrier;

    // Load default configurations for the controller
    Controller::Configuration configuration;

    // Directoy where client folders are located
    QString homeDirectory = QDir::currentPath();

    // Name of role
    QString name;

    if(parser.isSet(beFirstRole)) {

        qDebug() << "Running as first role";
        name = "first_role";

        Controller * firstRole = Runner::create_client(configuration,
                                                    true,
                                                    false,
                                                    homeDirectory,
                                                    name,
                                                    Coin::Seed("21234123421341234123412341234235467578679765456786543567854356765435678908765435678976546786709586078567095867085607865087065986"),
                                                    PUBLICONESYMMETRIC_BITCOIN_NETWORK,
                                                    &manager,
                                                    PUBLICONESYMMETRIC_BLOCKCYPHER_TOKEN);

        barrier.add(firstRole);

        QString savePath = cleanup_savepath(homeDirectory, name);

        // Setup buys and sells
        buy(firstRole, savePath, APOLLO_FILE);
        buy(firstRole, savePath, NASA_FILE);
        buy(firstRole, savePath, SFC_FILE);
        buy(firstRole, savePath, USNOW_FILE);

        sell(firstRole, torrentDataDirectory, HUBBLECAST_FILE);
        sell(firstRole, torrentDataDirectory, OPENSUSEISO_FILE);
        sell(firstRole, torrentDataDirectory, STOCKFOOTAGECHRISTMAS_FILE);

    } else {

        qDebug() << "Running as second role";
        name = "second_role";

        Controller * secondRole = Runner::create_client(configuration,
                                                    true,
                                                    false,
                                                    homeDirectory,
                                                    name,
                                                    Coin::Seed("55367543567543567467546657689376878988976589734857345834975893475934985390128038291038120983091839081203812098381209981381248931"),
                                                    PUBLICONESYMMETRIC_BITCOIN_NETWORK,
                                                    &manager,
                                                    PUBLICONESYMMETRIC_BLOCKCYPHER_TOKEN);

        barrier.add(secondRole);

        QString savePath = cleanup_savepath(homeDirectory, name);

        // Setup buys and sells
        buy(secondRole, savePath, HUBBLECAST_FILE);
        buy(secondRole, savePath, OPENSUSEISO_FILE);
        buy(secondRole, savePath, STOCKFOOTAGECHRISTMAS_FILE);

        sell(secondRole, torrentDataDirectory, APOLLO_FILE);
        sell(secondRole, torrentDataDirectory, NASA_FILE);
        sell(secondRole, torrentDataDirectory, SFC_FILE);
        sell(secondRole, torrentDataDirectory, USNOW_FILE);

    }

    // Wait for it to end
    barrier.erect();

    return 0;
}

void buy(Controller * controller, const QString & savePath, const QString & torrentFile) {

    BuyerTorrentPlugin::Configuration buyerPluginConfiguration(false,
                                                               5,         // Maximum piece price (satoshi)
                                                               4*3600,    // Maximum lock time on refund (seconds)
                                                               10000,     // BitCoinRepresentation NOT WORKING FOR WHATEVER REASON: BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.1).satoshies(), // Max fee per kB (satoshi)
                                                               1          // #sellers
                                                               );

    Controller::Torrent::Configuration buyerTorrentConfiguration = Runner::create_torrent_configuration(torrentFile, savePath);

    try {

        Coin::UnspentP2PKHOutput utxo = Runner::find_utxo((buyerTorrentConfiguration.torrentFile())->num_pieces(),
                                                          buyerPluginConfiguration,
                                                          controller->wallet());

        controller->addTorrent(buyerTorrentConfiguration, buyerPluginConfiguration, utxo);

    } catch (const std::runtime_error & e) {
        qDebug() << "Torrent not added to buyer.";
    }

}

void sell(Controller * controller, const QString & torrentDataDirectory, const QString & torrentFile) {

    SellerTorrentPlugin::Configuration sellerPluginConfiguration(false,
                                                                 2,       // Minimum piece price (satoshi)
                                                                 2*3600,   // Minimum lock time on refund (seconds)
                                                                 5000,     // BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.01).satoshies(), // Min fee per kB (satoshi)
                                                                 1,        // Max #seller
                                                                 17*60    // Maximum contract confirmation delay (seconds)
                                                                 );

    Controller::Torrent::Configuration sellerTorrentConfiguration = Runner::create_torrent_configuration(torrentFile, torrentDataDirectory);

    controller->addTorrent(sellerTorrentConfiguration, sellerPluginConfiguration);
}

QString cleanup_savepath(const QString & homeDirectory, const QString & name) {

    // Location where test clients create their home folders,
    // i.e. where they load/save actual wallet/torrent data
    QString savePath = homeDirectory + QDir::separator() + name + QDir::separator() + "torrent_data";

    QDir torrent_data(savePath);

    if(!torrent_data.exists()) {
        qDebug() << "Could not create folder:" << savePath;
        throw std::runtime_error("error...");
    }

    torrent_data.removeRecursively();

    return savePath;

}
