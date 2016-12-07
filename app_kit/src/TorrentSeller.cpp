#include <app_kit/TorrentSeller.hpp>
#include <app_kit/WorkerResult.hpp>
#include <core/TorrentPlugin.hpp>
#include <core/Torrent.hpp>
#include <core/Session.hpp>

namespace joystream {
namespace appkit {

TorrentSeller::TorrentSeller(QObject* parent, core::Node* node, bitcoin::SPVWallet* wallet,
                           std::shared_ptr<WorkerResult> result,
                           libtorrent::sha1_hash infoHash,
                           const protocol_session::SellingPolicy& policy,
                           const protocol_wire::SellerTerms& terms,
                           protocol_session::GenerateP2SHKeyPairCallbackHandler paychanKeysGenerator,
                           protocol_session::GenerateReceiveAddressesCallbackHandler receiveAddressesGenerator)
    : Worker(parent, infoHash, result, node),
      _wallet(wallet),
      _policy(policy),
      _terms(terms),
      _paychanKeysGenerator(paychanKeysGenerator),
      _receiveAddressesGenerator(receiveAddressesGenerator)
{

}

std::shared_ptr<WorkerResult> TorrentSeller::sell(QObject* parent, core::Node* node, bitcoin::SPVWallet* wallet,
                                                  libtorrent::sha1_hash infoHash,
                                                  const protocol_session::SellingPolicy& policy,
                                                  const protocol_wire::SellerTerms& terms,
                                                  protocol_session::GenerateP2SHKeyPairCallbackHandler paychanKeysGenerator,
                                                  protocol_session::GenerateReceiveAddressesCallbackHandler receiveAddressesGenerator) {

    auto result = std::make_shared<WorkerResult>(infoHash);

    new TorrentSeller(parent, node, wallet, result, infoHash, policy, terms, paychanKeysGenerator, receiveAddressesGenerator);

    return result;

}

void TorrentSeller::start() {

    auto torrent = getTorrent();

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

void TorrentSeller::startSelling() {

    auto torrent = getTorrent();

    if(!torrent) {
        finished(WorkerResult::Error::TorrentDoesNotExist);
        return;
    }

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

}
}
