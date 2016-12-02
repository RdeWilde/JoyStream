#ifndef TORRENTBUYER_HPP
#define TORRENTBUYER_HPP

#include <QObject>

#include <bitcoin/SPVWallet.hpp>
#include <core/Node.hpp>

#include <app_kit/BuyTorrentResponse.hpp>

namespace joystream {
namespace appkit {


class TorrentBuyer : public QObject
{
public:
    static std::shared_ptr<BuyTorrentResponse> buy(QObject* parent, core::Node*, bitcoin::SPVWallet*,
                                                   libtorrent::sha1_hash infoHash,
                                                   const protocol_session::BuyingPolicy& policy,
                                                   const protocol_wire::BuyerTerms& terms);
protected slots:
    void onTorrentStateChanged(libtorrent::torrent_status::state_t state, float progress);
    void onTorrentRemoved(const libtorrent::sha1_hash&);

private:
    TorrentBuyer(QObject* parent, core::Node*, bitcoin::SPVWallet*, std::shared_ptr<BuyTorrentResponse> response,
                 libtorrent::sha1_hash infoHash,
                 const protocol_session::BuyingPolicy& policy,
                 const protocol_wire::BuyerTerms& terms);

    bitcoin::SPVWallet* _wallet;
    core::Node* _node;
    libtorrent::sha1_hash _infoHash;
    protocol_session::BuyingPolicy _policy;
    protocol_wire::BuyerTerms _terms;
    std::shared_ptr<BuyTorrentResponse> _response;

    void start();

    void finished();
    void finished(BuyTorrentResponse::Error);
    void finished(std::exception_ptr);

    core::Torrent* getTorrentPointerOrFail();
    void startBuying();
    void startPlugin();

};

}
}

#endif // TORRENTBUYER_HPP
