#include <QCoreApplication>
#include <QFile>
#include <QDateTime>
#include <ctime>
#include <QTimer>

#include <bitcoin/SPVWallet.hpp>
#include <common/P2PKHAddress.hpp>

#include <CoinQ/CoinQ_coinparams.h>

#include <signal.h>

bool shuttingDown = false;

void handleSignal(int sig)
{
    shuttingDown = true;
}

using joystream::bitcoin::SPVWallet;

void sendAllCoinsToBlockCypherFaucet(joystream::bitcoin::SPVWallet & wallet) {
    // mwmabpJVisvti3WEP5vhFRtn3yqHRD9KNP

}

int main(int argc, char *argv[])
{
    if(getenv("DEBUG") != NULL) {
        INIT_LOGGER("netsync.log");
    }

    QCoreApplication a(argc, argv);

    joystream::bitcoin::SPVWallet wallet("test-store.db", "test-blocktree.dat", Coin::Network::testnet3);

    /*
    // Same seed as blockcypher-wallet test
    Coin::Seed seed("27891465891239001238391236589203948574567842549230457167823941893047812940123194312489312840923849010124893128409238490101248931");

    // Time when first tx was mined funding our wallet @ block height 716216
    // https://live.blockcypher.com/btc-testnet/tx/4f41e57c02d18b9bf65bd218439ac8a620df119fa4b964a3135f31ec00a3d176/
    // * When backing up a wallet we should make sure to preserve the created date as well as the seed
    QDateTime created = QDateTime::fromString("2016-02-19T07:52:31Z", Qt::ISODate);

    if(!QFile("test-store.db").exists()) {
        std::cout << "creating wallet...\n";
        wallet.create(seed, created.toTime_t());
        wallet.getReceiveAddress();
        wallet.getReceiveAddress();
        wallet.getReceiveAddress();
    } else {
        std::cout << "openning wallet...\n";
        wallet.open();
    }
    */

    if(!QFile("test-store.db").exists()) {
        std::cout << "creating wallet...\n";
        wallet.create();
    } else {
        std::cout << "openning wallet...\n";
        wallet.open();
    }

    if(!wallet.isInitialized()) {
        std::cerr << "Failed to open the wallet.\n";
        return -1;
    }

    std::cout << "Wallet contains the following receive addresses:\n";

    for(auto addr : wallet.listAddresses()) {
        std::cout << addr.toBase58CheckEncoding().toStdString() << std::endl;
    }

    std::cout << std::endl << "Balance: " << wallet.unconfirmedBalance() << std::endl;

    // Timer used for checking if a signal was caught..
    QTimer *timer = new QTimer();

    QObject::connect(timer, &QTimer::timeout, [&wallet, &timer](){
        if(!shuttingDown) return;
        timer->stop();
        std::cout << "Stopping..." << std::endl;
        wallet.stopSync();
    });

    timer->start(2000);
    signal(SIGINT, &handleSignal);
    signal(SIGTERM, &handleSignal);

    QObject::connect(&wallet, &SPVWallet::storeUpdateFailed, [](std::string err){
        std::cerr << "StoreError: " << err << std::endl;
        shuttingDown = true;
    });

    QObject::connect(&wallet, &SPVWallet::blockTreeUpdateFailed, [&wallet](std::string err){
        // This is something we can't really recover from, and we have to disconnect from the peer
        std::cerr << "BlockTree Error: " << err << std::endl;
        shuttingDown = true;
    });

    QObject::connect(&wallet, &SPVWallet::statusMessageUpdated, [](std::string message){
        std::cout << message << std::endl;
    });

    QObject::connect(&wallet, &SPVWallet::balanceChanged, [](uint64_t confirmed, uint64_t unconfirmed){
        std::cout << "confirmed balance: " << confirmed << std::endl;
        std::cout << "0-conf    balance: " << unconfirmed << std::endl;
    });

    QObject::connect(&wallet, &SPVWallet::disconnected, [](){
        std::cout << "Disconnected" << std::endl;
        shuttingDown = true;
    });

    QObject::connect(&wallet, &SPVWallet::statusChanged, [&wallet, &a, &timer](SPVWallet::wallet_status_t status){
       if(status == SPVWallet::wallet_status_t::SYNCHED)  {
           std::cout << std::endl << "Wallet Synched" << std::endl;
       }

       if(status == SPVWallet::wallet_status_t::SYNCHING_HEADERS) {
           std::cout << "synching headers" << std::endl;
       }

       if(status == SPVWallet::wallet_status_t::SYNCHING_BLOCKS) {
           std::cout << "synching blocks" << std::endl;
       }

       if(status == SPVWallet::wallet_status_t::CONNECTING) {
           std::cout << "Connecting.." << std::endl;
       }

       if(status == SPVWallet::wallet_status_t::OFFLINE) {
           // Exit application when netsync stops
           /* when computer wakes from sleep for some reason status
           doesn't goto OFFLINE after going to DISCONNECTED */

            if(shuttingDown) {
                std::cout << "Application Exiting..\n";
                a.exit();
            }
       }
    });

    // First argument should be a node running testnet3 on default port 18333
    // https://tpfaucet.appspot.com/
    // 52.4.156.236
    // testnet-seed.bitcoin.petertodd.org

    if(argc > 1) {
        wallet.sync(argv[1], 18333);
    } else {
        // if no peer provided use a peer by dns lookup
        wallet.sync("testnet-seed.bitcoin.petertodd.org", 18333);
    }

    // return a.exec(); // << results in seg fault and other undefined behaviour?!

    // No problems with this approach!
    int ret = a.exec();
    std::cout << "Exited application event loop with code: " << ret << std::endl;
    return ret;
}
