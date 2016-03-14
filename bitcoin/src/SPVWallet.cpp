#include <common/UnspentP2PKHOutput.hpp>
#include <common/P2PKHAddress.hpp>
#include <bitcoin/SPVWallet.hpp>

namespace joystream {
namespace bitcoin {

const CoinQ::CoinParams regtestParams(
        0xDAB5BFFA, 70001, "18444", 0x6f, 0xc4, "regtest", "regtest", 100000000, "testBTC", 21000000, 0,
        &sha256_2, &sha256_2,
        Coin::CoinBlockHeader(1, 1296688602, 0x207fffff, 2, uchar_vector(32, 0), uchar_vector("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")));

const CoinQ::CoinParams getCoinParamsForNetwork(Coin::Network network) {

    if(network == Coin::Network::mainnet) {
        return CoinQ::getBitcoinParams();

    } else if(network == Coin::Network::testnet3) {
        return CoinQ::getTestnet3Params();

    }else if(network == Coin::Network::regtest){
        return regtestParams;
    }

    throw std::runtime_error("network not supported");
}

SPVWallet::SPVWallet(QString storePath, Coin::Network network) :
  _storePath(storePath.toStdString()),
  _network(network),
  _networkSync(getCoinParamsForNetwork(network))
{
    _networkSync.subscribeStatus([this](const std::string& message)
    {

    });

    _networkSync.subscribeProtocolError([this](const std::string& error, int code)
    {

    });

    _networkSync.subscribeConnectionError([this](const std::string& error, int code)
    {

    });

    _networkSync.subscribeBlockTreeError([this](const std::string& error, int code)
    {

    });

    _networkSync.subscribeOpen([this]()
    {

    });

    _networkSync.subscribeClose([this]()
    {

    });

    _networkSync.subscribeStarted([this]()
    {

    });

    _networkSync.subscribeStopped([this]()
    {

    });

    _networkSync.subscribeTimeout([this]()
    {

    });

    _networkSync.subscribeSynchingHeaders([this]()
    {

    });

    _networkSync.subscribeHeadersSynched([this]()
    {

    });

    _networkSync.subscribeSynchingBlocks([this]()
    {

    });

    _networkSync.subscribeBlocksSynched([this]()
    {

    });

    _networkSync.subscribeAddBestChain([this](const chain_header_t& header)
    {

    });

    _networkSync.subscribeRemoveBestChain([this](const chain_header_t& header)
    {

    });

    _networkSync.subscribeNewTx([this](const Coin::Transaction& cointx)
    {

    });

    _networkSync.subscribeMerkleTx([this](const ChainMerkleBlock& chainmerkleblock, const Coin::Transaction& cointx, unsigned int txindex, unsigned int txcount)
    {

    });

    _networkSync.subscribeTxConfirmed([this](const ChainMerkleBlock& chainmerkleblock, const bytes_t& txhash, unsigned int txindex, unsigned int txcount)
    {

    });

    _networkSync.subscribeMerkleBlock([this](const ChainMerkleBlock& chainMerkleBlock)
    {

    });

    _networkSync.subscribeBlockTreeChanged([this]()
    {

    });
}

void SPVWallet::Create() {
    if(!_store.create(_storePath, _network)){
        throw std::runtime_error("unable to create store");
    }
}

void SPVWallet::Create(Coin::Seed seed) {
    if(!_store.create(_storePath, _network, seed, std::time(nullptr))){
        throw std::runtime_error("unable to create store");
    }
}

void SPVWallet::Open() {

    // Only open the store once
    if(!_store.connected()) {
        if(!_store.open(_storePath)) {
            throw std::runtime_error("failed to open wallet");
        }

        if(_network != _store.network()){
            throw std::runtime_error("store network type mistmatch");
        }
    }
}

void SPVWallet::Sync(std::string host, int port) {
    _networkSync.start(host, port);
}

Coin::PrivateKey SPVWallet::GetKey(bool createReceiveAddress) {
    Coin::PrivateKey sk = _store.getKey(createReceiveAddress);

    if(createReceiveAddress) {
        // update bloom filter
    }

    return sk;
}

std::vector<Coin::PrivateKey> SPVWallet::GetKeys(uint32_t numKeys, bool createReceiveAddress) {
    std::vector<Coin::PrivateKey> keys = _store.getKeys(numKeys, createReceiveAddress);
    if(createReceiveAddress) {
        for(auto & sk : keys) {
            // update bloom filter
        }
    }
    return keys;
}

std::vector<Coin::KeyPair>
SPVWallet::GetKeyPairs(uint32_t num_pairs, bool createReceiveAddress) {
    std::vector<Coin::KeyPair> keyPairs = _store.getKeyPairs(num_pairs, createReceiveAddress);
    if (createReceiveAddress) {
        for (auto & keypair : keyPairs) {
            // update bloom filter
        }
    }
    return keyPairs;
}

void SPVWallet::ReleaseKey(const Coin::PrivateKey &sk) {
    _store.releaseKey(sk);
}

Coin::P2PKHAddress
SPVWallet::GetReceiveAddress()
{
    Coin::P2PKHAddress addr = _store.getReceiveAddress();
    // update bloom filter
    return addr;
}


void SPVWallet::BroadcastTx(Coin::Transaction & tx) {
    _store.addTransaction(tx);
    _networkSync.sendTx(tx);
}

}
}
