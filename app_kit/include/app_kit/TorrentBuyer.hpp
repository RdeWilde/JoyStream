#ifndef TORRENTBUYER_HPP
#define TORRENTBUYER_HPP

#include <QObject>

#include <bitcoin/SPVWallet.hpp>
#include <core/Node.hpp>

#include <app_kit/Worker.hpp>
#include <app_kit/WorkerResult.hpp>

namespace joystream {
namespace appkit {


class TorrentBuyer : public Worker
{
public:
    static std::shared_ptr<WorkerResult> buy(QObject* parent, core::Node*, bitcoin::SPVWallet*,
                                             libtorrent::sha1_hash,
                                             const protocol_session::BuyingPolicy&,
                                             const protocol_wire::BuyerTerms&,
                                             protocol_session::GenerateP2SHKeyPairCallbackHandler,
                                             protocol_session::GenerateReceiveAddressesCallbackHandler,
                                             protocol_session::GenerateChangeAddressesCallbackHandler);

protected slots:
    void onTorrentStateChanged(libtorrent::torrent_status::state_t state, float progress);
    void onTorrentRemoved(const libtorrent::sha1_hash&);
    void start();
    void abort();

private:
    TorrentBuyer(QObject* parent, core::Node*, bitcoin::SPVWallet*, std::shared_ptr<WorkerResult> response,
                 libtorrent::sha1_hash,
                 const protocol_session::BuyingPolicy&,
                 const protocol_wire::BuyerTerms&,
                 protocol_session::GenerateP2SHKeyPairCallbackHandler,
                 protocol_session::GenerateReceiveAddressesCallbackHandler,
                 protocol_session::GenerateChangeAddressesCallbackHandler);

    bitcoin::SPVWallet* const _wallet;
    core::Node* const _node;
    const protocol_session::BuyingPolicy _policy;
    const protocol_wire::BuyerTerms _terms;
    std::shared_ptr<WorkerResult> _response;

    const protocol_session::GenerateP2SHKeyPairCallbackHandler _paychanKeysGenerator;
    const protocol_session::GenerateReceiveAddressesCallbackHandler _receiveAddressesGenerator;
    const protocol_session::GenerateChangeAddressesCallbackHandler _changeAddressesGenerator;

    void finished();
    void finished(WorkerResult::Error);
    void finished(std::exception_ptr);

    core::Torrent* getTorrentPointerOrFail();
    void startBuying();
    void startPlugin();

};

}
}

#endif // TORRENTBUYER_HPP
