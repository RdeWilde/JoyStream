#include <app_kit/Worker.hpp>
#include <QTimer>

namespace joystream {
namespace appkit {

std::map<libtorrent::sha1_hash, const Worker*> Worker::_workers;

Worker::Worker(QObject *parent, libtorrent::sha1_hash infoHash) :
    QObject(parent),
    _infoHash(infoHash)
{
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
}
}
