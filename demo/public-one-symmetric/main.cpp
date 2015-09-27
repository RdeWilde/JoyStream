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

/**
 * HOW TO RUN
 * ==============================================================
 *
 * This application requires two arguments
 *
 * a)
 *
 * b)
 *
 *
 *  * 1) Make sure tracker is running on local machine,
 *    and that all torrents in question have been loaded
 *    in as being tracked (but not seeded)
 *
 * Expects path to folder (without trailing /)
 * which contains two subfolders which you must have created
 * all the torrent files and torrent data,
 *              hence you must put the right content here.
 *
 * e.g: ./localonetoone /home/bedeho/JoyStream
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
        std::cerr << "Exactky two arguments must be provided.";
        return 1;
    }

    // Get first argument: Torrent file directory
    QString torrentFileDirectory = arguments.first();

    if(!QDir(torrentFileDirectory).exists()) {
        std::cerr << torrentFileDirectory.toStdString() << " directory does not exist.";
        return 1;
    }

    // Get second argument: Torrent data directory
    QString torrentDataDirectory = arguments[1];

    if(!QDir(torrentDataDirectory).exists()) {
        std::cerr << torrentDataDirectory.toStdString() << " directory does not exist.";
        return 1;
    }

    // Seperator used for paths
    std::string sep(QDir::separator() == QChar('/') ? "/" : "\\");

    // Dervie torrent file paths
    std::string torrent_path = torrentFileDirectory.toStdString() + sep;
    std::string RISE_AND_RISE_OF_BITCOIN_FILE   = torrent_path + "The.Rise.and.Rise.of.Bitcoin.2014.720p.HDrip.x264.AAC.MVGroup.org.mp4.torrent",
                LITTLE_SIMZ_FILE                = torrent_path + "Little Simz - AGE 101- DROP 1 - 01 Homosapiens VS Aliens.mp3.torrent",
                APOLLO_FILE                     = torrent_path + "apollo.torrent",
                HUBBLECAST_FILE                 = torrent_path + "hubblecast.torrent",
                NASA_FILE                       = torrent_path + "Nasa.torrent",
                OPENSUSEISO_FILE                = torrent_path + "OpenSUSEiso.torrent",
                SFC_FILE                        = torrent_path + "sfc.torrent",
                STOCKFOOTAGECHRISTMAS_FILE      = torrent_path + "stock footage christmas.torrent",
                USNOW_FILE                      = torrent_path + "us_now.torrent";

    // Location where test clients create their home folders,
    // i.e. where they load/save actual wallet/torrent data
    QString homeDirectory = QDir::currentPath();

    // Network access manager
    QNetworkAccessManager manager;

    // Create a controller tracker
    Runner::ControllerBarrier barrier;

    // Load default configurations for the controller
    Controller::Configuration configuration;

    // Buyer
    Controller * lone_buyer = Runner::create_client(configuration,
                                                    true,
                                                    false,
                                                    homeDirectory,
                                                    QString("lone_buyer"),
                                                    Coin::Seed("22222222245702754273542035678056807968507856907805687056875807896570586786507890586075806786709586078567095867085607865087065986"),
                                                    LOCALONETOONE_BITCOIN_NETWORK,
                                                    &manager,
                                                    LOCALONETOONE_BLOCKCYPHER_TOKEN);


    BuyerTorrentPlugin::Configuration(false,
                                       5,         // Maximum piece price (satoshi)
                                       4*3600,     // Maximum lock time on refund (seconds)
                                       10000,      // BitCoinRepresentation NOT WORKING FOR WHATEVER REASON: BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.1).satoshies(), // Max fee per kB (satoshi)
                                       1);          // #sellers

    /**
    lone_buyer->addTorrent(Runner::create_torrent_configuration(torrentInfo, QString::fromStdString(torrentsDirectory));,
                           pluginConfiguration,
                           find_utxo(torrentInfo.num_pieces(),

                            ,
                           lone_buyer->wallet());


*/



    // Wait for both to end
    barrier.erect();

    return 0;
}
