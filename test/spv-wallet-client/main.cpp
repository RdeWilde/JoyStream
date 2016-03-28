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
    std::cout << "Stopping..." << std::endl;
    shuttingDown = true;
}

using joystream::bitcoin::SPVWallet;

int main(int argc, char *argv[])
{
     INIT_LOGGER("netsync.log");

    QCoreApplication a(argc, argv);

    joystream::bitcoin::SPVWallet wallet("test-store.db", "test-blocktree.dat", Coin::Network::testnet3);

    // Same seed as blockcypher-wallet test
    Coin::Seed seed("27891465891239001238391236589203948574567842549230457167823941893047812940123194312489312840923849010124893128409238490101248931");

    // Time when first tx was mined funding our wallet @ block height 716216
    // https://live.blockcypher.com/btc-testnet/tx/4f41e57c02d18b9bf65bd218439ac8a620df119fa4b964a3135f31ec00a3d176/
    // * When backing up a wallet we should make sure to preserve the created date as well as the seed
    QDateTime created = QDateTime::fromString("2016-02-19T07:52:31Z", Qt::ISODate);

    std::cout << "openning wallet...\n";

    if(!QFile("test-store.db").exists()) {
        wallet.Create(seed, created.toTime_t());
        wallet.GetReceiveAddress();
        wallet.GetReceiveAddress();
    } else {
        wallet.Open();
    }


    // Timer used for checking if a signal was caught..
    QTimer *timer = new QTimer();

    QObject::connect(timer, &QTimer::timeout, [&wallet, &timer](){
        if(!shuttingDown) return;
        timer->stop();
        wallet.StopSync();
    });

    timer->start(2000);
    signal(SIGINT, &handleSignal);
    signal(SIGTERM, &handleSignal);


    QObject::connect(&wallet, &SPVWallet::StatusChanged, [&wallet, &a](SPVWallet::wallet_status_t status){
       if(status == SPVWallet::wallet_status_t::SYNCHED)  {
           // Display Balance
           std::cout << std::endl << "Wallet Balance: " << wallet.Balance() << std::endl;

           // Exit on next timer callback
           shuttingDown = true;
       }

       if(status == SPVWallet::wallet_status_t::SYNCHING_BLOCKS) {
           std::cout << ".";
       }

       if(status == SPVWallet::wallet_status_t::OFFLINE) {
           // Exit application when netsync stops
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
        wallet.Sync(argv[1], 18333);
    } else {
        // if no peer provided use a peer by dns lookup
        wallet.Sync("testnet-seed.bitcoin.petertodd.org", 18333);
    }

    // return a.exec(); // << results in seg fault and other undefined behaviour?!

    // No problems with this approach!
    int ret = a.exec();
    std::cout << "Exited application event loop with code: " << ret << std::endl;
    return ret;
}
