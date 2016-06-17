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
#include <QTimer>

#include <core/controller/Controller.hpp>
#include <core/logger/LoggerManager.hpp>
#include <common/Seed.hpp>
#include <gui/MainWindow.hpp>
#include <AutoUpdater.hpp>
#include <Analytics.hpp>
#include <InstanceManager.hpp>
#include <gui/GeneralLoadingProgressDialog.hpp>

#include <boost/asio/impl/src.hpp>

#define ERROR_LOG_ENDPOINT "error.joystream.co/error"
#define ERROR_LOG_MAX_SIZE 200*20

#define APPLICATION_WALLET_FILENAME "joystream.store"
#define APPLICATION_BLOCKTREE_FILENAME "joystream.blocktree"

bool send_errorlog(QFile * errorFile, QNetworkAccessManager * manager);
QString get_data_path(const QString & filename = QString());

// JoyStream entry point
int main(int argc, char* argv[]) {

    // Create directory used for saving user's application specific data
    QDir::home().mkdir(".joystream");

    // Move old wallet and blocktree to new directory
    QFile::rename(QDir::homePath() + QDir::separator() + APPLICATION_WALLET_FILENAME, get_data_path(APPLICATION_WALLET_FILENAME));
    QFile::rename(QDir::homePath() + QDir::separator() + APPLICATION_BLOCKTREE_FILENAME, get_data_path(APPLICATION_BLOCKTREE_FILENAME));

    // Cleanup old files
    QFile::remove(QDir::homePath() + QDir::separator() + "main.txt");
    QFile::remove(QDir::homePath() + QDir::separator() + "joystream-sync.log");
    QFile::remove(QDir::homePath() + QDir::separator() + "wallet.sqlite3");

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
    QCommandLineOption showFreshOption("f", "Create and use a fresh parameter file.");
    parser.addOption(showFreshOption);
    QCommandLineOption detachFromLauncherOption("d", "Detach from launcher");
    parser.addOption(detachFromLauncherOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    // This is mainly intended to be used on windows and linux when running joystream after an auto-update
    // to allow the updater to exit after starting joystream.
    if(parser.isSet(detachFromLauncherOption)) {
        QString program = QCoreApplication::applicationFilePath();
        std::cout << "Launching " << program.toStdString() << std::endl;
        QStringList arguments;
        arguments << "-n";
        QProcess::startDetached(program, arguments);
        return 0;
    }

    GeneralLoadingProgressDialog progressDialog(APPLICATION_NAME);
    progressDialog.updateMessage("Initializing");
    progressDialog.show();
    app.processEvents();

    InstanceManager instanceManager(APPLICATION_NAME);

    if(!instanceManager.isMain()) {
        qDebug() << "Another instance of JoyStream is already running.";
        return 0;
    }

    joystream::app::AutoUpdater au(app);

    // Call update manager, if allowed
    if(!parser.isSet(showNoUpdateOption)){
        progressDialog.updateMessage("Checking for updates...");
        if(au.newVersionAvailable()) {
            progressDialog.updateMessage("A new version is available, Starting Updater...");
            au.updateMiniUI();
            // Updater will again check for new version, without a UI so
            // Hang around a few seconds until the downloading dialog shows up
            QTimer::singleShot(3000, &app, SLOT(quit()));
            app.exec();
            return 0;
        }
        progressDialog.updateMessage("You are running latest version.");
        app.processEvents();
    }

    // Create logging category
    bool use_stdout_log = true;
    QLoggingCategory * category = global_log_manager.createLogger("main", get_data_path(), use_stdout_log, false);

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
            QString parameterFilePath = get_data_path(APPLICATION_PARAMETER_FILE_NAME);
            std::string fileString = parameterFilePath.toStdString();

            // Check that file exists, and that it actually is a file
            if(!QFile::exists(parameterFilePath)) {

                qDebug() << "WARNING: parameter file"
                         << fileString.c_str()
                         << "does not exist. Try fresh run option if problem persists";

                exit(EXIT_FAILURE);

            } else // Load state from file
                configuration = Controller::Configuration(fileString.c_str());
        }

        // Wallet location
        QString storePath = get_data_path(APPLICATION_WALLET_FILENAME);

        // Block Tree file location
        QString blocktreePath = get_data_path(APPLICATION_BLOCKTREE_FILENAME);

        // NetSync log file
        if(getenv("NETSYNC_LOGGER") != NULL) {
            QString syncLogPath = get_data_path("sync.log");
            INIT_LOGGER(syncLogPath.toStdString().c_str());
        }

        progressDialog.updateMessage("Starting Controller");

        // Create controller
        // Loading the block tree will take some time.. maybe we can show a progress dialogue
        // Or let the main window open the wallet..?
        Controller controller(configuration, &manager,
                              APPLICATION_BITCOIN_NETWORK,
                              APPLICATION_BLOCKCYPHER_TOKEN,
                              storePath,
                              blocktreePath,
                              *category);

        QObject::connect(&controller, &Controller::closed, &app, &QApplication::quit);

        // Allocate view and show it
        MainWindow view(&controller, "");

        QObject::connect(&instanceManager, &InstanceManager::activate, [&view](){
            qDebug() << "Got Activate Signal from Instance Manager";
            view.maximize();
            QApplication::setActiveWindow(&view);
        });

        progressDialog.hide();

        view.show();

        view.startUp([&app](std::string progress){
            app.processEvents();
        });

        // Create and start analytics tracking
        joystream::app::Analytics analytics(&manager, APPLICATION_MIXPANEL_TOKEN, applicationVersion); //, Analytics::_defaultAnalyticsHost);

        // Monitor controller
        analytics.monitor(&controller);

        // Send start signal and regular pings
        analytics.start(joystream::app::Analytics::_defaultPingMsInterval);

        if(controller.closing()) return 0;

        // Start event loop: this is the only Qt event loop in the entire application
        int retValue = app.exec();

        qDebug() << "Application event loop exited, application closing.";

        return retValue;

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
        send_errorlog(global_log_manager.loggers["main"].file, &manager);
        qCDebug((*category)) << "Shutting down, try to restart software, delete wallet file if problem persists.";

        return 1;
    }
}

QString get_data_path(const QString & filename) {
    return QDir::homePath() + QDir::separator() + ".joystream" + QDir::separator() + filename;
}

bool send_errorlog(QFile * errorFile, QNetworkAccessManager * manager) {

    if(!errorFile->exists())
            return false;

    if(!errorFile->open(QIODevice::ReadOnly))
            return false;

    QByteArray data = errorFile->readAll();

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
