/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QNetworkAccessManager>

#include <core/controller/Controller.hpp>
#include <core/logger/LoggerManager.hpp>
#include <common/Seed.hpp>
#include <gui/MainWindow.hpp>
#include <wallet/Manager.hpp>
#include <AutoUpdater.hpp>
#include <Analytics.hpp>

#define ERROR_LOG_ENDPOINT "error.joystream.co/error"
#define ERROR_LOG_MAX_SIZE 200*20

bool send_errorlog(QNetworkAccessManager * manager);
bool deleteWallet(QString filename);
void createWallet(QString filename);

// JoyStream entry point
int main(int argc, char* argv[]) {

    // Create Qt application: all objects created after this point are owned by this thread
    QApplication app(argc, argv);
    QApplication::setApplicationName(APPLICATION_NAME);

    QString applicationVersion = QString::number(APPLICATION_VERSION_MAJOR) + "." + QString::number(APPLICATION_VERSION_MINOR) + "." + QString::number(APPLICATION_VERSION_PATCH);
    QApplication::setApplicationVersion(applicationVersion);

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

    AutoUpdater au(app);

    // Call update manager, if allowed
    if(!parser.isSet(showNoUpdateOption) && au.newVersionAvailable()) {
         au.updateMiniUI();
         return 0;
    }

    // Check console flag
    bool showView = false;
    if(!parser.isSet(showConsoleModeOption))
            showView = true;

    // Create logging category
    bool use_stdout_logg = true;
    QLoggingCategory * category = global_log_manager.createLogger("main", use_stdout_logg, false);

    // Network access manager instance used by all code trying to use network
    QNetworkAccessManager manager;

    try {

        // Load default state
        Controller::Configuration configuration;

        // If fresh flag is not passed,
        // then open existing parameter file
        // DISABLED: !parser.isSet(showFreshOption)
        if(false) {

            // Get name of file name
            QString file = QDir::homePath() + QDir::separator() + APPLICATION_PARAMETER_FILE_NAME;
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

        // Load wallet
        QString walletFile = QDir::homePath() + QDir::separator() + QString("wallet.sqlite3");

        Wallet::Manager * wallet;

        // Create wallet if it does not exist
        if(!QFile(walletFile).exists()) {

            qCDebug((*category)) << "Creating a fresh wallet " << walletFile;
            createWallet(walletFile);

            wallet = new Wallet::Manager(walletFile);

        } else {

            // Load existing wallet
            wallet = new Wallet::Manager(walletFile);

            //check if the metadata version is set in the database
            //value of 0 means the database
            //was created in an earlier version of the app before key pooling was
            //implemented and we should drop it and create a new one
            if(wallet->version() == 0) {
                wallet->close();
                qCDebug((*category)) << "Purging old wallet, creating new fresh wallet " << walletFile;
                deleteWallet(walletFile);
                createWallet(walletFile);
                wallet = new Wallet::Manager(walletFile);
            }
        }

        // Initiliaze blockcypher interface
        wallet->BLOCKCYPHER_init(&manager, APPLICATION_BLOCKCYPHER_TOKEN);

        // Create controller
        Controller controller(configuration, wallet, &manager, *category);

        QObject::connect(&controller, &Controller::closed, &app, &QApplication::quit);

        // Allocate view and show it
        MainWindow view(&controller, wallet, "");
        view.show();

        // Create and start analytics tracking
        Analytics analytics(&manager, APPLICATION_MIXPANEL_TOKEN, applicationVersion); //, Analytics::_defaultAnalyticsHost);

        // Monitor controller
        analytics.monitor(&controller);

        // Send start signal and regular pings
        analytics.start(Analytics::_defaultPingMsInterval);

        // Start event loop: this is the only Qt event loop in the entire application
        app.exec();

        qDebug() << "Application event loop exited, application closing.";

        return 0;

    } catch(const std::exception & e) {

        /**
         * In the future:
         * 1) Sepearate exception handling for each part (view, wallet, contoller, configruations, etc)
         * 2) In each case, module should only throw exceptions which truly should be visible to outside
         * everything else should be logged and handled gracefully itnernally.
         */

        // This is temporary error handling fix for sending off logs to error logging endpoint error.joystream.co
        // Find more appropriate fix later

        qCDebug((*category)) << "Catastrophic error due to unhandled exception" << e.what();
        qCDebug((*category)) << "Sending error log to " << ERROR_LOG_ENDPOINT;
        send_errorlog(&manager);
        qCDebug((*category)) << "Shutting down, try to restart software, delete wallet file if problem persists.";

        return 1;
    }
}

bool deleteWallet(QString filename) {
    return QFile::remove(filename);
}

void createWallet(QString filename) {

    // Get new random seed
    Coin::Seed seed = Coin::Seed::generate();

    // Create wallet
    Wallet::Manager::createNewWallet(filename, APPLICATION_BITCOIN_NETWORK, seed);
}

bool send_errorlog(QNetworkAccessManager * manager) {

    // Read in error file
    QFile errorFile(QDir::homePath() + QDir::separator() + QString("main.txt"));

    if(!errorFile.exists())
            return false;

    if(!errorFile.open(QIODevice::ReadOnly))
            return false;

    QByteArray data = errorFile.readAll();

    // cut off excessive part
    data = data.right(ERROR_LOG_MAX_SIZE);

    // Machine specs
    data += "\n";
    data += "JoyStream Version:"        + QCoreApplication::applicationVersion() + "\n";
    data += "buildAbi():"               + QSysInfo::buildAbi() + "\n";
    data += "buildCpuArchitecture():"   + QSysInfo::buildCpuArchitecture() + "\n";
    data += "currentCpuArchitecture():" + QSysInfo::currentCpuArchitecture() + "\n";
    data += "kernelType():"             + QSysInfo::kernelType() + "\n";
    data += "kernelVersion():"          + QSysInfo::kernelVersion() + "\n";
    data += "prettyProductName():"      + QSysInfo::prettyProductName() + "\n";
    data += "productType():"            + QSysInfo::productType() + "\n";
    data += "productVersion():"         + QSysInfo::productVersion() + "\n";

    // Create url and POST file
    QUrl url(QString("http://") + ERROR_LOG_ENDPOINT);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply * reply = manager->post(request, data);

    // Block until we have reply finished
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();

    return reply->error() == QNetworkReply::NoError;
}
