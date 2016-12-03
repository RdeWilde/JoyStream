#include <app_kit/TorrentBuyer.hpp>
#include <app_kit/BuyTorrentResponse.hpp>
#include <core/TorrentPlugin.hpp>
#include <core/Torrent.hpp>

namespace joystream {
namespace appkit {

std::map<libtorrent::sha1_hash, TorrentBuyer*> TorrentBuyer::_workers;

TorrentBuyer::TorrentBuyer(QObject* parent, core::Node* node, bitcoin::SPVWallet* wallet,
                           std::shared_ptr<BuyTorrentResponse> response,
                           libtorrent::sha1_hash infoHash,
                           const protocol_session::BuyingPolicy& policy,
                           const protocol_wire::BuyerTerms& terms)
    : QObject(parent),
      _node(node),
      _wallet(wallet),
      _policy(policy),
      _terms(terms),
      _infoHash(infoHash),
      _response(response)
{

    QObject::connect(this, &TorrentBuyer::destroyed, _response.get(), &BuyTorrentResponse::finishedProcessing);

    // Only one buyer per infohash
    if(_workers.find(_infoHash) != _workers.end()) {

        QTimer::singleShot(0, this, &TorrentBuyer::abort);

    } else {

        _workers[_infoHash] = this;

        QObject::connect(_node, &core::Node::removedTorrent, this, &TorrentBuyer::onTorrentRemoved);

        QTimer::singleShot(0, this, &TorrentBuyer::start);
    }
}

std::shared_ptr<BuyTorrentResponse> TorrentBuyer::buy(QObject* parent, core::Node* node, bitcoin::SPVWallet* wallet,
                                        libtorrent::sha1_hash infoHash,
                                        const protocol_session::BuyingPolicy& policy,
                                        const protocol_wire::BuyerTerms& terms) {

    auto response = std::make_shared<BuyTorrentResponse>(infoHash);

    new TorrentBuyer(parent, node, wallet, response, infoHash, policy, terms);

    return response;

}

void TorrentBuyer::abort() {
    _response->setError(BuyTorrentResponse::Error::AlreadyTryingToBuyTorrent);
    delete this;
}

void TorrentBuyer::finished() {
    _workers.erase(_infoHash);
    delete this;
}

void TorrentBuyer::finished(BuyTorrentResponse::Error e) {
    _response->setError(e);
    finished();
}

void TorrentBuyer::finished(std::exception_ptr e) {
    _response->setError(e);
    finished();
}

core::Torrent* TorrentBuyer::getTorrentPointerOrFail() {
    auto torrents = _node->torrents();

    if(torrents.find(_infoHash) == torrents.end()) {
        _response->setError(BuyTorrentResponse::Error::TorrentDoesNotExist);
        finished();
        return nullptr;
    }

    return torrents[_infoHash];
}

void TorrentBuyer::start() {

    auto torrent = getTorrentPointerOrFail();

    if(!torrent)
        return;

    if(!torrent->torrentPluginSet()) {
        _response->setError(BuyTorrentResponse::Error::TorrentPluginNotSet);
        finished();
        return;
    }

    if(_wallet->locked()) {
        _response->setError(BuyTorrentResponse::Error::WalletLocked);
        finished();
        return;
    }

    auto state = torrent->state();

    if (libtorrent::torrent_status::state_t::seeding == state) {
        _response->setError(BuyTorrentResponse::Error::TorrentAlreadyDownloaded);
        finished();
        return;
    }

    // Ready to download ?
    if (libtorrent::torrent_status::state_t::downloading == state) {
        startBuying();
    }

    QObject::connect(torrent, &joystream::core::Torrent::stateChanged, this, &TorrentBuyer::onTorrentStateChanged);
}

void TorrentBuyer::onTorrentStateChanged(libtorrent::torrent_status::state_t state, float progress) {
    if(libtorrent::torrent_status::state_t::seeding == state) {
        _response->setError(BuyTorrentResponse::Error::TorrentAlreadyDownloaded);
        finished();
        return;
    }

    if(libtorrent::torrent_status::state_t::downloading == state) {
        startBuying();
    }
}

void TorrentBuyer::onTorrentRemoved(const libtorrent::sha1_hash &info_hash) {
    if(_infoHash != info_hash)
        return;

    finished();
}

void TorrentBuyer::startBuying() {

    auto torrent = getTorrentPointerOrFail();

    if(!torrent)
        return;

    if(!torrent->torrentPluginSet()) {
        _response->setError(BuyTorrentResponse::Error::TorrentPluginNotSet);
        finished();
        return;
    }

    if(_wallet->locked()) {
        _response->setError(BuyTorrentResponse::Error::WalletLocked);
        finished();
        return;
    }

    auto ti = torrent->metaData().lock();

    auto minimumFunds = joystream::protocol_session::Session<libtorrent::tcp::endpoint>::minimumFundsRequiredAsBuyer(_terms, ti->num_pieces());

    std::cout << "trying to lock minimumFunds: " << minimumFunds << std::endl;

    auto outputs = _wallet->lockOutputs(minimumFunds, 0);

    if(outputs.size() == 0) {
        _response->setError(BuyTorrentResponse::Error::UnableToLockFunds);
        finished();
        return;
    }

    core::TorrentPlugin* plugin = torrent->torrentPlugin();

    plugin->toBuyMode(
        // protocol_session::GenerateP2SHKeyPairCallbackHandler
        [this](const protocol_session::P2SHScriptGeneratorFromPubKey& generateScript, const uchar_vector& data) -> Coin::KeyPair {

            Coin::PrivateKey sk = _wallet->generateKey([&generateScript, &data](const Coin::PublicKey & pk){
                return bitcoin::RedeemScriptInfo(generateScript(pk), data);
            });

            return Coin::KeyPair(sk);
        },
        // protocol_session::GenerateReceiveAddressesCallbackHandler
        [this](int npairs) -> std::vector<Coin::P2PKHAddress> {
            std::vector<Coin::P2PKHAddress> addresses;

            for(int n = 0; n < npairs; n++) {
                addresses.push_back(_wallet->generateReceiveAddress());
            }

            return addresses;
        },
        // protocol_session::GenerateChangeAddressesCallbackHandler
        [this](int npairs) -> std::vector<Coin::P2PKHAddress> {
            std::vector<Coin::P2PKHAddress> addresses;

            for(int n = 0; n < npairs; n++) {
                addresses.push_back(_wallet->generateChangeAddress());
            }

            return addresses;
        },
        // Coin::UnspentOutputSet
        outputs,
        _policy,
        _terms,
        [this, outputs](const std::exception_ptr & e) {
            if(e) {
                _wallet->unlockOutputs(outputs);
                _response->setError(e);
                finished();
            } else {
                startPlugin();
            }
        });
}

void TorrentBuyer::startPlugin() {

    auto torrent = getTorrentPointerOrFail();

    if(!torrent)
        return;

    torrent->torrentPlugin()->start([this](const std::exception_ptr &eptr){
        if(eptr){
            _response->setError(eptr);
        }else {
            // success
        }
        finished();
    });
}

}
}
