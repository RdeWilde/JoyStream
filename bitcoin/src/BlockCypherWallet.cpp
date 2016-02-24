#include <bitcoin/BlockCypherWallet.hpp>

namespace joystream {
namespace bitcoin {

BlockCypherWallet::BlockCypherWallet(QString storePath, Coin::Network network,
                                     BlockCypher::Client * restClient,
                                     BlockCypher::WebSocketClient * wsClient) :
    _storePath(storePath.toStdString()),
    _network(network),
    _restClient(restClient),
    _wsClient(wsClient)
{
    // Make sure the clients are talking to the correct network
    if(network != _wsClient->network() || network != _restClient->network()) {
        throw std::runtime_error("client network mistmatch");
    }

    _utxoManager = BlockCypher::UTXOManager::createManager(_wsClient, network);

    QObject::connect(_utxoManager, &BlockCypher::UTXOManager::balanceChanged,
                     this, &BlockCypherWallet::BalanceChanged);
}

void BlockCypherWallet::Create() {
    if(!_store.create(_storePath, _network)){
        throw std::runtime_error("unable to create store");
    }
}

void BlockCypherWallet::Create(Coin::Seed seed) {
    if(!_store.create(_storePath, _network, seed, std::time(nullptr))){
        throw std::runtime_error("unable to create store");
    }
}

void BlockCypherWallet::Open() {

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

bool BlockCypherWallet::Sync(uint tries) {
    std::list<Coin::P2PKHAddress> addresses = _store.listReceiveAddresses();

    if(addresses.size() == 0 || tries == 0) {
        return true;
    }

    for(uint i = 0; i < tries; i++){
        if(_utxoManager->refreshUtxoState(_restClient, addresses)) {
            return true;
        }
    }

    return false;
}

Coin::PrivateKey BlockCypherWallet::GetKey(bool createReceiveAddress) {
    Coin::PrivateKey sk = _store.getKey(createReceiveAddress);

    if(createReceiveAddress)
        _utxoManager->addAddress(sk.toPublicKey().toP2PKHAddress(_network));

    return sk;
}

std::vector<Coin::PrivateKey> BlockCypherWallet::GetKeys(uint32_t numKeys, bool createReceiveAddress) {
    std::vector<Coin::PrivateKey> keys = _store.getKeys(numKeys, createReceiveAddress);
    if(createReceiveAddress) {
        for(auto & sk : keys) {
            _utxoManager->addAddress(sk.toPublicKey().toP2PKHAddress(_network));
        }
    }
    return keys;
}

std::vector<Coin::KeyPair>
BlockCypherWallet::GetKeyPairs(uint32_t num_pairs, bool createReceiveAddress) {
    std::vector<Coin::KeyPair> keyPairs = _store.getKeyPairs(num_pairs, createReceiveAddress);
    if (createReceiveAddress) {
        for (auto & keypair : keyPairs) {
            _utxoManager->addAddress(keypair.pk().toP2PKHAddress(_network));
        }
    }
    return keyPairs;
}

void BlockCypherWallet::ReleaseKey(const Coin::PrivateKey &sk) {
    _store.releaseKey(sk);
}

Coin::P2PKHAddress
BlockCypherWallet::GetReceiveAddress()
{
    Coin::P2PKHAddress addr = _store.getReceiveAddress();
    _utxoManager->addAddress(addr);
    return addr;
}

std::list<Coin::UnspentP2PKHOutput>
BlockCypherWallet::GetUnspentOutputs(uint64_t minValue, uint32_t minimalConfirmatinos, uint32_t currentBlockHeight)
{
    std::set<BlockCypher::UTXO> utxos = _utxoManager->getUtxoSet(minValue, minimalConfirmatinos, currentBlockHeight);

    std::list<Coin::UnspentP2PKHOutput> unspentOutputs;

    for (const BlockCypher::UTXO &utxo : utxos) {
        Coin::PrivateKey sk;
        if(_store.loadKey(Coin::P2PKHAddress::fromBase58CheckEncoding(utxo.address()), sk)) {
            Coin::KeyPair keypair(sk);
            Coin::UnspentP2PKHOutput output(keypair, utxo.outPoint(), utxo.value());
            unspentOutputs.push_back(output);
        }
    }

    return unspentOutputs;
}

void BlockCypherWallet::ReleaseUnspentOutputs(const std::list<Coin::UnspentP2PKHOutput> outputs)
{
    std::set<BlockCypher::UTXO> utxos;

    for (auto & output : outputs) {
        BlockCypher::UTXO utxo(output.keyPair().pk().toP2PKHAddress(_network).toBase58CheckEncoding(),
                               output.outPoint(), output.value());
        utxos.insert(utxo);
    }

    _utxoManager->releaseUtxoSet(utxos);
}

}
}
