/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <QApplication>
#include <QCommandLineParser>
#include <QString>
#include <QDir>
#include <QNetworkAccessManager>

#include <core/controller/Controller.hpp>
#include <core/logger/LoggerManager.hpp>
#include <common/Seed.hpp>
#include <gui/MainWindow.hpp>
#include <wallet/Manager.hpp>

// JoyStream entry point
int main(int argc, char* argv[]) {

    // Create Qt application: all objects created after this point are owned by this thread
    QApplication app(argc, argv);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(QString::number(APPLICATION_VERSION_MAJOR) + "." + QString::number(APPLICATION_VERSION_MINOR));

    // Setup command line parsing
    QCommandLineParser parser;
    parser.setApplicationDescription(APPLICATION_DESCRIPTION);

    // Add options
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption showNoUpdateOption("n", "Do not run update manager.");
    parser.addOption(showNoUpdateOption);
    QCommandLineOption showConsoleModeOption("c", "Run in console mode.");
    parser.addOption(showConsoleModeOption);
    QCommandLineOption showFreshOption("f", "Create and use a fresh parameter file.");
    parser.addOption(showFreshOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    // Call update manager, if allowed
    if(!parser.isSet(showNoUpdateOption)) {

        /**
        bool doRestart = updateManager();

        if(doRestart) {
            // Call update runner
        }
        */

    }

    // Check console flag
    bool showView = false;
    if(!parser.isSet(showConsoleModeOption))
        showView = true;

    // Load default state
    Controller::Configuration configuration;

    // If fresh flag is not passed,
    // then open existing parameter file
    if(!parser.isSet(showFreshOption)) {

        // Get name of file name
        QString file = QDir::current().absolutePath() + QDir::separator() + PARAMETER_FILE_NAME;
        std::string fileString = file.toStdString();

        // Check that file exists, and that it actually is a file
        if(!QFile::exists(file)) {

            qDebug() << "WARNING: parameter file"
                     << fileString.c_str()
                     << "does not exist. Try fresh run option if problem persists";

            exit(EXIT_FAILURE);

        } else // Load state from file
            configuration = Controller::Configuration(fileString.c_str());
    }

    // Create logging category
    bool use_stdout_logg = true;
    QLoggingCategory * category = global_log_manager.createLogger("main", use_stdout_logg, false);

    // Load wallet
    QString walletFile = QDir::current().absolutePath() + QDir::separator() + QString("wallet.sqlite3");

    // Create wallet if it does not exist
    if(!QFile(walletFile).exists()) {

        qCDebug((*category)) << "Creating a fresh wallet " << walletFile;

        // Get predefined seed
        //Coin::Seed seed = Coin::Seed::testSeeds[wallet_seed_counter];
        //Q_ASSERT(wallet_seed_counter <= Coin::Seed::testSeeds.size());

        // Get new random seed
        Coin::Seed seed = Coin::Seed::generate();

        // Create wallet
        Wallet::Manager::createNewWallet(walletFile, BITCOIN_NETWORK, seed);
    }

    // Network access manager instance used by all code trying to use network
    QNetworkAccessManager manager;

    // Load and wallet
    Wallet::Manager wallet(walletFile);

    // Initiliaze blockcypher interface
    wallet.BLOCKCYPHER_init(&manager);

    // Create controller
    Controller controller(configuration, &wallet, &manager, *category);

    QObject::connect(&controller,
                     &Controller::closed,
                     &app,
                     &QApplication::quit);

    // Allocate view and show it
    MainWindow view(&controller, &wallet, "");
    view.show();

    // Start event loop: this is the only Qt event loop in the entire application
    app.exec();

    qDebug() << "Application event loop exited, application closing.";

    return 0;
}
