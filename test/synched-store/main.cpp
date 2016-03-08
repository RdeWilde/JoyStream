#include <QCoreApplication>
#include <bitcoin/SynchedStore.hpp>
#include <QFile>
#include <QDateTime>
#include <ctime>

#include <common/P2PKHAddress.hpp>

int main(int argc, char *argv[])
{
    INIT_LOGGER("netsync.log");

    QCoreApplication a(argc, argv);

    joystream::bitcoin::Store store;
    joystream::bitcoin::SynchedStore synched(CoinQ::getTestnet3Params());

    // Same seed as blockcypher-wallet test
    Coin::Seed seed("27891465891239001238391236589203948574567842549230457167823941893047812940123194312489312840923849010124893128409238490101248931");

    // Time when first tx was mined funding our wallet @ block height 716216
    // https://live.blockcypher.com/btc-testnet/tx/4f41e57c02d18b9bf65bd218439ac8a620df119fa4b964a3135f31ec00a3d176/
    // * When backing up a wallet we should make sure to preserve the created date as well as the seed
    QDateTime created = QDateTime::fromString("2016-02-19T07:52:31Z", Qt::ISODate);

    if(!QFile("test-synched-store.db").exists()) {
        store.create("test-synched-store.db", Coin::Network::testnet3, seed, created.toTime_t());
        store.getReceiveAddress();
        store.getReceiveAddress();
    } else {
        store.open("test-synched-store.db");
    }

    synched.useStore(&store);

    // initialise blocktree - will create it and sync if doesn't exist
    synched.loadHeaders("blocktree.dat");

    // Set flag = 1 : -> dynamiccaly updates bloom filter to find inputs spending matched outpoints
    // details at https://github.com/bitcoin/bips/blob/master/bip-0037.mediawiki
    synched.setFilterParams(0.001, 0, 0);

    // First argument is a node running testnet3 on default port 18333
    // Good example found here: https://tpfaucet.appspot.com/
    // 52.4.156.236
    synched.startSync(argv[1], 18333);

    return a.exec();
}

