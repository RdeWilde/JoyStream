#include "sellqueue.hpp"
#include <app_kit/HelperFunctions.hpp>

SellQueue::SellQueue(joystream::appkit::AppKit *kit)
    : _kit(kit)
{
    // wait for torrents to be added
    QObject::connect(_kit->node(), &joystream::core::Node::addedTorrent, [this](joystream::core::Torrent * torrent){

        // Only handle torrents in our queue
        if(_queue.find(torrent->infoHash()) == _queue.end())
            return;

        // wait for torrent plugins to be added
        QObject::connect(torrent, &joystream::core::Torrent::torrentPluginAdded, [this, torrent](joystream::core::TorrentPlugin *plugin){

            auto infoHash = torrent->infoHash();

            if(_queue.find(torrent->infoHash()) == _queue.end())
                return;

            torrent->resume([this, torrent](const std::exception_ptr &e) {});

            // ready to download
            if(libtorrent::torrent_status::state_t::downloading == torrent->state()) {

                // we cannot sell torrent its not yet downloaded
                 // remove the torrent from the map
                _queue.erase(infoHash);
                return;
            }

            // ready to upload?
            if(libtorrent::torrent_status::state_t::seeding == torrent->state()) {
                // We already have the torrent..
                sellTorrent(torrent->infoHash());
                return;
            }

            // otherwise.. wait for valid state
            QObject::connect(torrent, &joystream::core::Torrent::stateChanged, [this, torrent](libtorrent::torrent_status::state_t state, float progress){
                auto infoHash = torrent->infoHash();

                if(_queue.find(infoHash) == _queue.end())
                    return;

                if(libtorrent::torrent_status::state_t::downloading == state) {
                    // we cannot sell torrent its not yet downloaded
                     // remove the torrent from the map
                    _queue.erase(infoHash);
                    return;
                }

                if(libtorrent::torrent_status::state_t::seeding == state) {
                    sellTorrent(infoHash);
                }
            });

        });
    });

}

void SellQueue::add(libtorrent::sha1_hash infohash, joystream::protocol_wire::SellerTerms terms, joystream::protocol_session::SellingPolicy policy) {
    Item item;
    item.policy = policy;
    item.terms = terms;
    _queue[infohash] = item;
}

void SellQueue::sellTorrent(libtorrent::sha1_hash infoHash) {
    auto torrents = _kit->node()->torrents();

    if(torrents.find(infoHash) == torrents.end()) {
        _queue.erase(infoHash);
        return;
    }

    if(_queue.find(infoHash) == _queue.end())
        return;

    joystream::core::Torrent* torrent = torrents[infoHash];
    auto item = _queue[infoHash];

    item.tries++;

    _kit->sellTorrent(torrent, item.policy, item.terms, [this, torrent, infoHash](const std::exception_ptr &eptr){
        if(eptr){
            // try again in 5 seconds..
            QTimer::singleShot(5000, [this, infoHash](){
                sellTorrent(infoHash);
            });
        } else {
            startTorrent(torrent);
        }
    });
}

void SellQueue::startTorrent(joystream::core::Torrent *torrent) {
    torrent->torrentPlugin()->start([this, torrent](const std::exception_ptr &eptr){
        if(eptr){
           std::rethrow_exception(eptr);
        }else {
            std::cout << "Torrent Started" << std::endl;
            _queue.erase(torrent->infoHash());
        }
    });
}
