#ifndef SELLQUEUE_HPP
#define SELLQUEUE_HPP

#include <libtorrent/sha1_hash.hpp>
#include <protocol_wire/SellerTerms.hpp>
#include <protocol_session/SellingPolicy.hpp>
#include <app_kit/AppKit.hpp>

class SellQueue
{
    struct Item {
        joystream::protocol_wire::SellerTerms terms;
        joystream::protocol_session::SellingPolicy policy;
        joystream::protocol_session::SessionState state;
        uint tries = 0;
        QMetaObject::Connection pluginAddedConnection;
        QMetaObject::Connection stateChangedConnection;
    };

public:
    SellQueue(joystream::appkit::AppKit* kit);

    void add(libtorrent::sha1_hash, joystream::protocol_wire::SellerTerms terms, joystream::protocol_session::SellingPolicy policy, joystream::protocol_session::SessionState);

private:
    std::map<libtorrent::sha1_hash, Item> _queue;
    joystream::appkit::AppKit *_kit;

    void sellTorrent(libtorrent::sha1_hash);
    void startTorrent(joystream::core::Torrent*);

    std::function<void(joystream::core::Torrent *plugin)>
        onTorrentAdded();

    std::function<void(joystream::core::TorrentPlugin *plugin)>
        onTorrentPluginAdded(joystream::core::Torrent* torrent);

    std::function<void(libtorrent::torrent_status::state_t, float)>
        onTorrentStateChanged(joystream::core::Torrent* torrent);

};

#endif // SELLQUEUE_HPP
