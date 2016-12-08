#include <app_kit/TorrentSeller.hpp>
#include <app_kit/WorkerResult.hpp>
#include <core/TorrentPlugin.hpp>
#include <core/Torrent.hpp>
#include <core/Session.hpp>

namespace joystream {
namespace appkit {

TorrentSeller::TorrentSeller(QObject* parent, core::Node* node, bitcoin::SPVWallet* wallet,
                           std::shared_ptr<WorkerResult> response,
                           libtorrent::sha1_hash infoHash,
                           const protocol_session::SellingPolicy& policy,
                           const protocol_wire::SellerTerms& terms,
                           protocol_session::GenerateP2SHKeyPairCallbackHandler paychanKeysGenerator,
                           protocol_session::GenerateReceiveAddressesCallbackHandler receiveAddressesGenerator)
    : Worker(parent, infoHash),
      _node(node),
      _wallet(wallet),
      _policy(policy),
      _terms(terms),
      _response(response),
      _paychanKeysGenerator(paychanKeysGenerator),
      _receiveAddressesGenerator(receiveAddressesGenerator)
{
    QObject::connect(this, &TorrentSeller::destroyed, _response.get(), &WorkerResult::finishedProcessing);
}

std::shared_ptr<WorkerResult> TorrentSeller::sell(QObject* parent, core::Node* node, bitcoin::SPVWallet* wallet,
                                                  libtorrent::sha1_hash infoHash,
                                                  const protocol_session::SellingPolicy& policy,
                                                  const protocol_wire::SellerTerms& terms,
                                                  protocol_session::GenerateP2SHKeyPairCallbackHandler paychanKeysGenerator,
                                                  protocol_session::GenerateReceiveAddressesCallbackHandler receiveAddressesGenerator) {

    auto response = std::make_shared<WorkerResult>(infoHash);

    new TorrentSeller(parent, node, wallet, response, infoHash, policy, terms, paychanKeysGenerator, receiveAddressesGenerator);

    return response;

}

void TorrentSeller::abort() {
    _response->setError(WorkerResult::Error::AlreadyTryingToSellTorrent);
    delete this;
}

void TorrentSeller::finished() {
    delete this;
}

void TorrentSeller::finished(WorkerResult::Error e) {
    _response->setError(e);
    finished();
}

void TorrentSeller::finished(std::exception_ptr e) {
    _response->setError(e);
    finished();
}

core::Torrent* TorrentSeller::getTorrentPointerOrFail() {
    auto torrents = _node->torrents();

    if(torrents.find(infoHash()) == torrents.end()) {
        finished(WorkerResult::Error::TorrentDoesNotExist);
        return nullptr;
    }

    return torrents[infoHash()];
}

void TorrentSeller::start() {

    QObject::connect(_node, &core::Node::removedTorrent, this, &TorrentSeller::onTorrentRemoved);

    auto torrent = getTorrentPointerOrFail();

    if(!torrent) {
        finished(WorkerResult::Error::TorrentDoesNotExist);
        return;
    }

    if(!torrent->torrentPluginSet()) {
        finished(WorkerResult::Error::TorrentPluginNotSet);
        return;
    }

    core::Session* session = torrent->torrentPlugin()->session();

    if(session->sellingSet()){
        finished(WorkerResult::Error::TorrentAlreadyInSellSession);
        return;
    }

    auto state = torrent->state();

    if (libtorrent::torrent_status::state_t::downloading == state) {
        finished(WorkerResult::Error::TorrentNotFullyDownloadedYet);
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
        finished(WorkerResult::Error::TorrentNotFullyDownloadedYet);
        return;
    }

    if(libtorrent::torrent_status::state_t::seeding == state) {
        startSelling();
    }
}

void TorrentSeller::onTorrentRemoved(const libtorrent::sha1_hash &info_hash) {
    if(infoHash() != info_hash)
        return;

    finished(WorkerResult::Error::TorrentDoesNotExist);
}

void TorrentSeller::startSelling() {

    auto torrent = getTorrentPointerOrFail();

    if(!torrent)
        return;

    if(!torrent->torrentPluginSet()) {
        finished(WorkerResult::Error::TorrentPluginNotSet);
        return;
    }

    if(_wallet->locked()) {
        finished(WorkerResult::Error::WalletLocked);
        return;
    }

    core::TorrentPlugin* plugin = torrent->torrentPlugin();

    plugin->toSellMode(_paychanKeysGenerator, _receiveAddressesGenerator, _policy, _terms,
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
        finished(WorkerResult::Error::TorrentDoesNotExist);
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
