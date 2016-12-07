#include <app_kit/TorrentObserver.hpp>
#include <core/TorrentPlugin.hpp>
#include <core/Session.hpp>

namespace joystream {
namespace appkit {


TorrentObserver::TorrentObserver(QObject* parent, core::Node* node, libtorrent::sha1_hash infoHash, std::shared_ptr<WorkerResult> result) :
    Worker(parent, infoHash, result),
    _node(node)
{
}

std::shared_ptr<WorkerResult> TorrentObserver::observe(QObject *parent, core::Node *node, libtorrent::sha1_hash infoHash) {
    auto result = std::make_shared<WorkerResult>(infoHash);

    new TorrentObserver(parent, node, infoHash, result);

    return result;
}

core::Torrent* TorrentObserver::getTorrent() {
    auto torrents = _node->torrents();

    if(torrents.find(infoHash()) == torrents.end()) {
        return nullptr;
    }

    return torrents[infoHash()];
}

void TorrentObserver::start() {
    QObject::connect(_node, &joystream::core::Node::removedTorrent, this, &TorrentObserver::finishIfTorrentRemoved);

    auto torrent = getTorrent();

    if(!torrent) {
        finished(WorkerResult::Error::TorrentDoesNotExist);
        return;
    }

    if(!torrent->torrentPluginSet()) {
        finished(WorkerResult::Error::TorrentPluginNotSet);
        return;
    }

    auto plugin = torrent->torrentPlugin();

    core::Session* session = plugin->session();

    if(session->mode() == protocol_session::SessionMode::observing) {
        finished(WorkerResult::Error::TorrentAlreadyInObservingMode);
        return;
    }

    plugin->toObserveMode([this](std::exception_ptr eptr){
        if(eptr) {
            finished(eptr);
        } else {
            finished();
        }
    });

}

}
}
