#ifndef CLIAPP_HPP
#define CLIAPP_HPP

#include <SignalHandler.hpp>
#include <app_kit/kit.hpp>
#include <common/Network.hpp>
#include <QCoreApplication>
#include <string>

class CliApp : public SignalHandler {

public:

    CliApp(int argc, char *argv[], const std::string &dataDir, Coin::Network network);

    int run();

public slots:
    void handleSignal(int signal);

private:

    void parseArgs();
    void createAppKitInstance();
    void processTorrent();
    void dumpWalletInfo();
    void claimRefunds(uint64_t txFee);

    static QByteArray readDataFromFile(QString filePath);
    static joystream::appkit::SavedTorrents deserializeSavedTorrents(QByteArray data);

    void loadTorrents();

    static QByteArray serializeSavedTorrents(joystream::appkit::SavedTorrents savedTorrents);
    static void writeDataToFile(QString filePath, QByteArray data);
    static void saveTorrentsToFile(joystream::appkit::SavedTorrents savedTorrents, QString savePath);

    QCoreApplication _app;
    std::string _command;
    std::string _argument;
    joystream::appkit::Settings _appKitSettings;
    joystream::appkit::DataDirectory _dataDir;
    std::unique_ptr<joystream::appkit::AppKit> _kit;
    const Coin::Network _network;
    bool _shuttingDown;

};

#endif // CLIAPP_HPP
