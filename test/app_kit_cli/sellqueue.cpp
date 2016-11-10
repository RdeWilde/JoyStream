#include "sellqueue.hpp"
#include <app_kit/HelperFunctions.hpp>

SellQueue::SellQueue(joystream::appkit::AppKit *kit)
    : _kit(kit)
{
    // wait for torrents to be added
    QObject::connect(_kit->node(), &joystream::core::Node::addedTorrent, onTorrentAdded());

}

void SellQueue::add(libtorrent::sha1_hash infohash,
                    joystream::protocol_wire::SellerTerms terms,
                    joystream::protocol_session::SellingPolicy policy,
                    joystream::protocol_session::SessionState state) {

    bool onlyUpdate = false;

    if(_queue.count(infohash)) {
        onlyUpdate = true;
    }

    Item item;
    item.policy = policy;
    item.terms = terms;
    item.state = state;
    _queue[infohash] = item;

    if(onlyUpdate)
        return;

    auto torrents = _kit->node()->torrents();

    if(torrents.count(infohash)) {
        auto torrent = torrents[infohash];

        if(item.pluginAddedConnection) torrent->disconnect(item.pluginAddedConnection);
        if(item.stateChangedConnection) torrent->disconnect(item.stateChangedConnection);

        if(torrent->torrentPluginSet()) {
            onTorrentPluginAdded(torrent)(torrent->torrentPlugin());
        }else {
            // wait for torrent plugin to be added
            item.stateChangedConnection = QObject::connect(torrent, &joystream::core::Torrent::torrentPluginAdded, onTorrentPluginAdded(torrent));
        }
    }
}

std::function<void(joystream::core::Torrent*)>
SellQueue::onTorrentAdded() {
    return [this](joystream::core::Torrent * torrent){

        auto infoHash = torrent->infoHash();

        // Only handle torrents in our queue
        if(_queue.find(infoHash) == _queue.end())
            return;

        // wait for torrent plugins to be added
        _queue[infoHash].pluginAddedConnection = QObject::connect(torrent, &joystream::core::Torrent::torrentPluginAdded, onTorrentPluginAdded(torrent));
    };
}

std::function<void(joystream::core::TorrentPlugin*)>
SellQueue::onTorrentPluginAdded(joystream::core::Torrent *torrent) {

    return [this, torrent](joystream::core::TorrentPlugin *plugin){

        auto infoHash = torrent->infoHash();

        if(_queue.find(infoHash) == _queue.end())
            return;

        torrent->resume([](const std::exception_ptr &e) {});

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
            sellTorrent(infoHash);
            return;
        }

        // otherwise.. wait for valid state
        _queue[infoHash].stateChangedConnection = QObject::connect(torrent, &joystream::core::Torrent::stateChanged, onTorrentStateChanged(torrent));
    };
}

std::function<void (libtorrent::torrent_status::state_t, float)>
SellQueue::onTorrentStateChanged(joystream::core::Torrent* torrent) {

    return [this, torrent](libtorrent::torrent_status::state_t state, float progress) -> void {
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
    };
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
    Item item = _queue[infoHash];

    if(item.state != joystream::protocol_session::SessionState::started)
        return;

    item.tries++;

    _kit->sellTorrent(torrent, item.policy, item.terms, [this, torrent, infoHash](const std::exception_ptr &eptr){
        if(eptr){
            // retry
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
        }
    });
}
