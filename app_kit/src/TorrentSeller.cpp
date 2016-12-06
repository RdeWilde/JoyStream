#include <app_kit/TorrentSeller.hpp>
#include <app_kit/SellTorrentResponse.hpp>
#include <core/TorrentPlugin.hpp>
#include <core/Torrent.hpp>
#include <core/Session.hpp>

namespace joystream {
namespace appkit {

std::map<libtorrent::sha1_hash, TorrentSeller*> TorrentSeller::_workers;

TorrentSeller::TorrentSeller(QObject* parent, core::Node* node, bitcoin::SPVWallet* wallet,
                           std::shared_ptr<SellTorrentResponse> response,
                           libtorrent::sha1_hash infoHash,
                           const protocol_session::SellingPolicy& policy,
                           const protocol_wire::SellerTerms& terms)
    : QObject(parent),
      _node(node),
      _wallet(wallet),
      _policy(policy),
      _terms(terms),
      _infoHash(infoHash),
      _response(response)
{

    QObject::connect(this, &TorrentSeller::destroyed, _response.get(), &SellTorrentResponse::finishedProcessing);

    // Only one buyer per infohash
    if(_workers.find(_infoHash) != _workers.end()) {

        QTimer::singleShot(0, this, &TorrentSeller::abort);

    } else {

        _workers[_infoHash] = this;

        QObject::connect(_node, &core::Node::removedTorrent, this, &TorrentSeller::onTorrentRemoved);

        QTimer::singleShot(0, this, &TorrentSeller::start);
    }
}

TorrentSeller::~TorrentSeller() {
    if(_workers.find(_infoHash) != _workers.end() && _workers[_infoHash] == this) {
        _workers.erase(_infoHash);
    }
}

std::shared_ptr<SellTorrentResponse> TorrentSeller::sell(QObject* parent, core::Node* node, bitcoin::SPVWallet* wallet,
                                                         libtorrent::sha1_hash infoHash,
                                                         const protocol_session::SellingPolicy& policy,
                                                         const protocol_wire::SellerTerms& terms) {

    auto response = std::make_shared<SellTorrentResponse>(infoHash);

    new TorrentSeller(parent, node, wallet, response, infoHash, policy, terms);

    return response;

}

void TorrentSeller::abort() {
    _response->setError(SellTorrentResponse::Error::AlreadyTryingToSellTorrent);
    delete this;
}

void TorrentSeller::finished() {
    delete this;
}

void TorrentSeller::finished(SellTorrentResponse::Error e) {
    _response->setError(e);
    finished();
}

void TorrentSeller::finished(std::exception_ptr e) {
    _response->setError(e);
    finished();
}

core::Torrent* TorrentSeller::getTorrentPointerOrFail() {
    auto torrents = _node->torrents();

    if(torrents.find(_infoHash) == torrents.end()) {
        finished(SellTorrentResponse::Error::TorrentDoesNotExist);
        return nullptr;
    }

    return torrents[_infoHash];
}

void TorrentSeller::start() {

    auto torrent = getTorrentPointerOrFail();

    if(!torrent) {
        finished(SellTorrentResponse::Error::TorrentDoesNotExist);
        return;
    }

    if(!torrent->torrentPluginSet()) {
        finished(SellTorrentResponse::Error::TorrentPluginNotSet);
        return;
    }

    core::Session* session = torrent->torrentPlugin()->session();

    if(session->sellingSet()){
        finished(SellTorrentResponse::Error::TorrentAlreadyInSellSession);
        return;
    }

    auto state = torrent->state();

    if (libtorrent::torrent_status::state_t::downloading == state) {
        finished(SellTorrentResponse::Error::TorrentNotFullyDownloadedYet);
        return;
    }

    // Ready to seed ?
    if (libtorrent::torrent_status::state_t::seeding == state) {
        startSelling();
    }

    QObject::connect(torrent, &joystream::core::Torrent::stateChanged, this, &TorrentSeller::onTorrentStateChanged);
}

void TorrentSeller::onTorrentStateChanged(libtorrent::torrent_status::state_t state, float progress) {
    if(libtorrent::torrent_status::state_t::downloading == state) {
        finished(SellTorrentResponse::Error::TorrentNotFullyDownloadedYet);
        return;
    }

    if(libtorrent::torrent_status::state_t::seeding == state) {
        startSelling();
    }
}

void TorrentSeller::onTorrentRemoved(const libtorrent::sha1_hash &info_hash) {
    if(_infoHash != info_hash)
        return;

    finished(SellTorrentResponse::Error::TorrentDoesNotExist);
}

void TorrentSeller::startSelling() {

    auto torrent = getTorrentPointerOrFail();

    if(!torrent)
        return;

    if(!torrent->torrentPluginSet()) {
        finished(SellTorrentResponse::Error::TorrentPluginNotSet);
        return;
    }

    if(_wallet->locked()) {
        finished(SellTorrentResponse::Error::WalletLocked);
        return;
    }

    core::TorrentPlugin* plugin = torrent->torrentPlugin();

    // Make a copy of the wallet pointer to use in the lambda
    auto wallet = _wallet;

    plugin->toSellMode(
        // protocol_session::GenerateP2SHKeyPairCallbackHandler
        [wallet](const protocol_session::P2SHScriptGeneratorFromPubKey& generateScript, const uchar_vector& data) -> Coin::KeyPair {

            Coin::PrivateKey sk = wallet->generateKey([&generateScript, &data](const Coin::PublicKey & pk){
                return bitcoin::RedeemScriptInfo(generateScript(pk), data);
            });

            return Coin::KeyPair(sk);
        },
        // protocol_session::GenerateReceiveAddressesCallbackHandler
        [wallet](int npairs) -> std::vector<Coin::P2PKHAddress> {
            std::vector<Coin::P2PKHAddress> addresses;

            for(int n = 0; n < npairs; n++) {
                addresses.push_back(wallet->generateReceiveAddress());
            }

            return addresses;
        },
        _policy,
        _terms,
        [this](const std::exception_ptr & e) {
            if(e) {
                finished(e);
            } else {
                startPlugin();
            }
        });
}

void TorrentSeller::startPlugin() {

    auto torrent = getTorrentPointerOrFail();

    if(!torrent) {
        finished(SellTorrentResponse::Error::TorrentDoesNotExist);
        return;
    }

    torrent->torrentPlugin()->start([this](const std::exception_ptr &eptr){
        if(eptr){
            finished(eptr);
        } else {
            finished();
        }
    });
}

}
}
