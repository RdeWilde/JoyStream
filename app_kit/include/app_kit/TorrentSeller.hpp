#ifndef TORRENTSELLER_HPP
#define TORRENTSELLER_HPP

#include <QObject>

#include <bitcoin/SPVWallet.hpp>
#include <core/Node.hpp>

#include <app_kit/SellTorrentResponse.hpp>

namespace joystream {
namespace appkit {


class TorrentSeller : public QObject
{
public:
    static std::shared_ptr<SellTorrentResponse> sell(QObject* parent, core::Node*, bitcoin::SPVWallet*,
                                                     libtorrent::sha1_hash infoHash,
                                                     const protocol_session::SellingPolicy& policy,
                                                     const protocol_wire::SellerTerms& terms);
    ~TorrentSeller();

protected slots:
    void onTorrentStateChanged(libtorrent::torrent_status::state_t state, float progress);
    void onTorrentRemoved(const libtorrent::sha1_hash&);

private:
    TorrentSeller(QObject* parent, core::Node*, bitcoin::SPVWallet*, std::shared_ptr<SellTorrentResponse> response,
                 libtorrent::sha1_hash infoHash,
                 const protocol_session::SellingPolicy& policy,
                 const protocol_wire::SellerTerms& terms);

    bitcoin::SPVWallet* _wallet;
    core::Node* _node;
    libtorrent::sha1_hash _infoHash;
    protocol_session::SellingPolicy _policy;
    protocol_wire::SellerTerms _terms;
    std::shared_ptr<SellTorrentResponse> _response;

    static std::map<libtorrent::sha1_hash, TorrentSeller*> _workers;

    Q_INVOKABLE void start();
    Q_INVOKABLE void abort();

    void finished();
    void finished(SellTorrentResponse::Error);
    void finished(std::exception_ptr);

    core::Torrent* getTorrentPointerOrFail();
    void startSelling();
    void startPlugin();

};

}
}

#endif // TORRENTSELLER_HPP
