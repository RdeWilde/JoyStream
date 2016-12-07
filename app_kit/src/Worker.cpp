#include <app_kit/Worker.hpp>
#include <QTimer>

namespace joystream {
namespace appkit {

std::map<libtorrent::sha1_hash, const Worker*> Worker::_workers;

Worker::Worker(QObject *parent, libtorrent::sha1_hash infoHash, std::shared_ptr<WorkerResult> result) :
    QObject(parent),
    _infoHash(infoHash),
    _result(result)
{
    QObject::connect(this, &Worker::destroyed, _result.get(), &WorkerResult::finishedProcessing);

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
