#ifndef BUYQUEUE_HPP
#define BUYQUEUE_HPP

#include <libtorrent/sha1_hash.hpp>
#include <protocol_wire/BuyerTerms.hpp>
#include <protocol_session/BuyingPolicy.hpp>
#include <app_kit/AppKit.hpp>

class BuyQueue
{
    struct Item {
        joystream::protocol_wire::BuyerTerms terms;
        joystream::protocol_session::BuyingPolicy policy;
        joystream::protocol_session::SessionState state;
        uint tries = 0;
        QMetaObject::Connection pluginAddedConnection;
        QMetaObject::Connection stateChangedConnection;
    };

public:
    BuyQueue(joystream::appkit::AppKit* kit);

    void add(libtorrent::sha1_hash, joystream::protocol_wire::BuyerTerms terms, joystream::protocol_session::BuyingPolicy policy, joystream::protocol_session::SessionState);

private:
    std::map<libtorrent::sha1_hash, Item> _queue;
    joystream::appkit::AppKit *_kit;

    void buyTorrent(libtorrent::sha1_hash);
    void startTorrent(joystream::core::Torrent*);

    std::function<void(joystream::core::Torrent *plugin)>
        onTorrentAdded();

    std::function<void(libtorrent::torrent_status::state_t, float)>
        onTorrentStateChanged(joystream::core::Torrent* torrent);

    std::function<void(joystream::core::TorrentPlugin *plugin)>
        onTorrentPluginAdded(joystream::core::Torrent* torrent);
};

#endif // BUYQUEUE_HPP
