#include <app_kit/Worker.hpp>
#include <QTimer>

namespace joystream {
namespace appkit {

std::map<libtorrent::sha1_hash, const Worker*> Worker::_workers;

Worker::Worker(QObject *parent, libtorrent::sha1_hash infoHash, std::shared_ptr<WorkerResult> result, core::Node *node) :
    QObject(parent),
    _infoHash(infoHash),
    _result(result),
    _node(node)
{
    QObject::connect(this, &Worker::destroyed, _result.get(), &WorkerResult::finishedProcessing);
    QObject::connect(_node, &core::Node::removedTorrent, this, &Worker::finishIfTorrentRemoved);

    // Only one worker per infohash
    if(_workers.find(_infoHash) != _workers.end()) {

        QTimer::singleShot(0, this, &Worker::abort);

    } else {

        _workers[_infoHash] = this;

        QTimer::singleShot(0, this, &Worker::start);
    }
}

Worker::~Worker() {
    if(_workers.find(_infoHash) != _workers.end() && _workers[_infoHash] == this) {
        _workers.erase(_infoHash);
    }
}

libtorrent::sha1_hash Worker::infoHash() const {
    return _infoHash;
}

core::Node* Worker::node() const {
    return _node;
}

core::Torrent* Worker::getTorrent() const {
    auto torrents = _node->torrents();

    if(torrents.find(infoHash()) == torrents.end()) {
        return nullptr;
    }

    return torrents[infoHash()];
}

void Worker::startPlugin() {

    auto torrent = getTorrent();

    if(!torrent) {
        finished(WorkerResult::Error::TorrentDoesNotExist);
        return;
    }

    if(!torrent->torrentPluginSet()) {
        finished(WorkerResult::Error::TorrentPluginNotSet);
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

void Worker::abort() {
    _result->setError(WorkerResult::Error::TorrentInAnotherWorker);
    delete this;
}

void Worker::finished() {
    delete this;
}

void Worker::finished(libtorrent::error_code ec)
{
    _result->setError(ec);
    finished();
}

void Worker::finished(WorkerResult::Error e) {
    _result->setError(e);
    finished();
}

void Worker::finished(std::exception_ptr e) {
    _result->setError(e);
    finished();
}

void Worker::finishIfTorrentRemoved(const libtorrent::sha1_hash &info_hash) {
    if(info_hash != infoHash())
        return;

    finished(WorkerResult::Error::TorrentRemoved);
}

}
}
