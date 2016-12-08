#include <app_kit/TorrentBuyer.hpp>
#include <app_kit/WorkerResult.hpp>
#include <core/TorrentPlugin.hpp>
#include <core/Torrent.hpp>
#include <core/Session.hpp>

namespace joystream {
namespace appkit {

TorrentBuyer::TorrentBuyer(QObject* parent, core::Node* node, bitcoin::SPVWallet* wallet,
                           std::shared_ptr<WorkerResult> response,
                           libtorrent::sha1_hash infoHash,
                           const protocol_session::BuyingPolicy& policy,
                           const protocol_wire::BuyerTerms& terms,
                           protocol_session::GenerateP2SHKeyPairCallbackHandler paychanKeysGenerator,
                           protocol_session::GenerateReceiveAddressesCallbackHandler receiveAddressesGenerator,
                           protocol_session::GenerateChangeAddressesCallbackHandler changeAddressesGenerator)
    : Worker(parent, infoHash),
      _node(node),
      _wallet(wallet),
      _policy(policy),
      _terms(terms),
      _response(response),
      _paychanKeysGenerator(paychanKeysGenerator),
      _receiveAddressesGenerator(receiveAddressesGenerator),
      _changeAddressesGenerator(changeAddressesGenerator)
{
    QObject::connect(this, &TorrentBuyer::destroyed, _response.get(), &WorkerResult::finishedProcessing);
}

std::shared_ptr<WorkerResult> TorrentBuyer::buy(QObject* parent, core::Node* node, bitcoin::SPVWallet* wallet,
                                                      libtorrent::sha1_hash infoHash,
                                                      const protocol_session::BuyingPolicy& policy,
                                                      const protocol_wire::BuyerTerms& terms,
                                                      protocol_session::GenerateP2SHKeyPairCallbackHandler paychanKeysGenerator,
                                                      protocol_session::GenerateReceiveAddressesCallbackHandler receiveAddressesGenerator,
                                                      protocol_session::GenerateChangeAddressesCallbackHandler changeAddressesGenerator) {

    auto response = std::make_shared<WorkerResult>(infoHash);

    new TorrentBuyer(parent, node, wallet, response, infoHash, policy, terms, paychanKeysGenerator, receiveAddressesGenerator, changeAddressesGenerator);

    return response;

}

void TorrentBuyer::abort() {
    _response->setError(WorkerResult::Error::AlreadyTryingToBuyTorrent);
    delete this;
}

void TorrentBuyer::finished() {
    delete this;
}

void TorrentBuyer::finished(WorkerResult::Error e) {
    _response->setError(e);
    finished();
}

void TorrentBuyer::finished(std::exception_ptr e) {
    _response->setError(e);
    finished();
}

core::Torrent* TorrentBuyer::getTorrentPointerOrFail() {
    auto torrents = _node->torrents();

    if(torrents.find(infoHash()) == torrents.end()) {
        finished(WorkerResult::Error::TorrentDoesNotExist);
        return nullptr;
    }

    return torrents[infoHash()];
}

void TorrentBuyer::start() {

    QObject::connect(_node, &core::Node::removedTorrent, this, &TorrentBuyer::onTorrentRemoved);

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

    if(session->buyingSet()){
        finished(WorkerResult::Error::TorrentAlreadyInBuySession);
        return;
    }

    auto state = torrent->state();

    if (libtorrent::torrent_status::state_t::seeding == state) {
        finished(WorkerResult::Error::TorrentAlreadyDownloaded);
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
        finished(WorkerResult::Error::TorrentAlreadyDownloaded);
        return;
    }

    if(libtorrent::torrent_status::state_t::downloading == state) {
        startBuying();
    }
}

void TorrentBuyer::onTorrentRemoved(const libtorrent::sha1_hash &info_hash) {
    if(infoHash() != info_hash)
        return;

    finished(WorkerResult::Error::TorrentDoesNotExist);
}

void TorrentBuyer::startBuying() {

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

    auto ti = torrent->metaData().lock();

    auto minimumFunds = joystream::protocol_session::Session<libtorrent::tcp::endpoint>::minimumFundsRequiredAsBuyer(_terms, ti->num_pieces());

    std::cout << "trying to lock minimumFunds: " << minimumFunds << std::endl;

    auto outputs = _wallet->lockOutputs(minimumFunds, 0);

    if(outputs.size() == 0) {
        finished(WorkerResult::Error::UnableToLockFunds);
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
