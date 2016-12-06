#include <app_kit/TorrentBuyer.hpp>
#include <app_kit/BuyTorrentResponse.hpp>
#include <core/TorrentPlugin.hpp>
#include <core/Torrent.hpp>
#include <core/Session.hpp>

namespace joystream {
namespace appkit {

std::map<libtorrent::sha1_hash, TorrentBuyer*> TorrentBuyer::_workers;

TorrentBuyer::TorrentBuyer(QObject* parent, core::Node* node, bitcoin::SPVWallet* wallet,
                           std::shared_ptr<BuyTorrentResponse> response,
                           libtorrent::sha1_hash infoHash,
                           const protocol_session::BuyingPolicy& policy,
                           const protocol_wire::BuyerTerms& terms,
                           protocol_session::GenerateP2SHKeyPairCallbackHandler paychanKeysGenerator,
                           protocol_session::GenerateReceiveAddressesCallbackHandler receiveAddressesGenerator,
                           protocol_session::GenerateChangeAddressesCallbackHandler changeAddressesGenerator)
    : QObject(parent),
      _node(node),
      _wallet(wallet),
      _policy(policy),
      _terms(terms),
      _infoHash(infoHash),
      _response(response),
      _paychanKeysGenerator(paychanKeysGenerator),
      _receiveAddressesGenerator(receiveAddressesGenerator),
      _changeAddressesGenerator(changeAddressesGenerator)
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

TorrentBuyer::~TorrentBuyer() {
    if(_workers.find(_infoHash) != _workers.end() && _workers[_infoHash] == this) {
        _workers.erase(_infoHash);
    }
}

std::shared_ptr<BuyTorrentResponse> TorrentBuyer::buy(QObject* parent, core::Node* node, bitcoin::SPVWallet* wallet,
                                                      libtorrent::sha1_hash infoHash,
                                                      const protocol_session::BuyingPolicy& policy,
                                                      const protocol_wire::BuyerTerms& terms,
                                                      protocol_session::GenerateP2SHKeyPairCallbackHandler paychanKeysGenerator,
                                                      protocol_session::GenerateReceiveAddressesCallbackHandler receiveAddressesGenerator,
                                                      protocol_session::GenerateChangeAddressesCallbackHandler changeAddressesGenerator) {

    auto response = std::make_shared<BuyTorrentResponse>(infoHash);

    new TorrentBuyer(parent, node, wallet, response, infoHash, policy, terms, paychanKeysGenerator, receiveAddressesGenerator, changeAddressesGenerator);

    return response;

}

void TorrentBuyer::abort() {
    _response->setError(BuyTorrentResponse::Error::AlreadyTryingToBuyTorrent);
    delete this;
}

void TorrentBuyer::finished() {
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
        finished(BuyTorrentResponse::Error::TorrentDoesNotExist);
        return nullptr;
    }

    return torrents[_infoHash];
}

void TorrentBuyer::start() {

    auto torrent = getTorrentPointerOrFail();

    if(!torrent) {
        finished(BuyTorrentResponse::Error::TorrentDoesNotExist);
        return;
    }

    if(!torrent->torrentPluginSet()) {
        finished(BuyTorrentResponse::Error::TorrentPluginNotSet);
        return;
    }

    core::Session* session = torrent->torrentPlugin()->session();

    if(session->buyingSet()){
        finished(BuyTorrentResponse::Error::TorrentAlreadyInBuySession);
        return;
    }

    auto state = torrent->state();

    if (libtorrent::torrent_status::state_t::seeding == state) {
        finished(BuyTorrentResponse::Error::TorrentAlreadyDownloaded);
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
        finished(BuyTorrentResponse::Error::TorrentAlreadyDownloaded);
        return;
    }

    if(libtorrent::torrent_status::state_t::downloading == state) {
        startBuying();
    }
}

void TorrentBuyer::onTorrentRemoved(const libtorrent::sha1_hash &info_hash) {
    if(_infoHash != info_hash)
        return;

    finished(BuyTorrentResponse::Error::TorrentDoesNotExist);
}

void TorrentBuyer::startBuying() {

    auto torrent = getTorrentPointerOrFail();

    if(!torrent)
        return;

    if(!torrent->torrentPluginSet()) {
        finished(BuyTorrentResponse::Error::TorrentPluginNotSet);
        return;
    }

    if(_wallet->locked()) {
        finished(BuyTorrentResponse::Error::WalletLocked);
        return;
    }

    auto ti = torrent->metaData().lock();

    auto minimumFunds = joystream::protocol_session::Session<libtorrent::tcp::endpoint>::minimumFundsRequiredAsBuyer(_terms, ti->num_pieces());

    std::cout << "trying to lock minimumFunds: " << minimumFunds << std::endl;

    auto outputs = _wallet->lockOutputs(minimumFunds, 0);

    if(outputs.size() == 0) {
        finished(BuyTorrentResponse::Error::UnableToLockFunds);
        return;
    }

    core::TorrentPlugin* plugin = torrent->torrentPlugin();

    plugin->toBuyMode(_paychanKeysGenerator, _receiveAddressesGenerator, _changeAddressesGenerator,
        outputs,
        _policy,
        _terms,
        [this, outputs](const std::exception_ptr & e) {
            if(e) {
                _wallet->unlockOutputs(outputs);
                finished(e);
            } else {
                startPlugin();
            }
        });
}

void TorrentBuyer::startPlugin() {

    auto torrent = getTorrentPointerOrFail();

    if(!torrent) {
        finished(BuyTorrentResponse::Error::TorrentDoesNotExist);
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
