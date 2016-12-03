#include <app_kit/TorrentAdder.hpp>
#include <core/Exception.hpp>

#include <QTimer>

namespace joystream {
namespace appkit {

// Initialize private static workers set
std::map<libtorrent::sha1_hash, TorrentAdder*> TorrentAdder::_workers;

TorrentAdder::TorrentAdder(QObject* parent,
                           core::Node* node,
                           TorrentAddRequest request,
                           std::shared_ptr<TorrentAddResponse> response)
    :  QObject(parent),
       _node(node),
       _request(request),
       _response(response) {

    QObject::connect(this, &TorrentAdder::destroyed, _response.get(), &TorrentAddResponse::finishedProcessing);

    // Only one adder per infohash
    if(_workers.find(_request.torrentIdentifier.infoHash()) != _workers.end()) {

        QTimer::singleShot(0, this, &TorrentAdder::abort);

    } else {

        _workers[_request.torrentIdentifier.infoHash()] = this;

        QObject::connect(_node, &joystream::core::Node::removedTorrent, this, &TorrentAdder::onTorrentRemoved);

        // Delay starting the worker to allow user to connect signals to the response object
        QTimer::singleShot(0, this, &TorrentAdder::start);
    }
}

void TorrentAdder::abort()
{
    _response->setError(TorrentAddResponse::Error::TorrentAlreadyBeingAdded);
    delete this;
}

std::shared_ptr<TorrentAddResponse> TorrentAdder::add(QObject* parent,
                                                      core::Node* node,
                                                      TorrentAddRequest request)
{

    auto response = std::make_shared<TorrentAddResponse>(request.torrentIdentifier.infoHash());

    new TorrentAdder(parent, node, request, response);

    return response;
}

void TorrentAdder::start() {

    QObject::connect(_node, &joystream::core::Node::addedTorrent, this, &TorrentAdder::onTorrentAdded);

    try {
        _node->addTorrent(_request.uploadLimit,
                         _request.downloadLimit,
                         _request.name,
                         _request.resumeData,
                         _request.savePath, _request.paused, _request.torrentIdentifier,
                         [this](libtorrent::error_code &ecode, libtorrent::torrent_handle &th) {
            addTorrentCallback(ecode, th);
        });

    } catch (core::exception::TorrentAlreadyExists &e) {
        finished(TorrentAddResponse::Error::TorrentAlreadyExists);
    }
}

void TorrentAdder::finished()
{
    _workers.erase(_request.torrentIdentifier.infoHash());
    delete this;
}

void TorrentAdder::finished(libtorrent::error_code ec)
{
    _response->setError(ec);

    finished();
}

void TorrentAdder::finished(TorrentAddResponse::Error err)
{
    _response->setError(err);

    finished();
}

void TorrentAdder::added() {
    _response->setAdded();
}

void TorrentAdder::onTorrentAdded(core::Torrent *torrent) {

    if (torrent->infoHash() != _request.torrentIdentifier.infoHash())
        return;

    // wait for torrent plugin to be added
    QObject::connect(torrent, &joystream::core::Torrent::torrentPluginAdded, this, &TorrentAdder::onTorrentPluginAdded);
}

void TorrentAdder::onTorrentRemoved(const libtorrent::sha1_hash &info_hash) {
    if(info_hash != _request.torrentIdentifier.infoHash())
        return;

    finished(TorrentAddResponse::Error::TorrentRemovedBeforePluginWasAdded);
}

void TorrentAdder::onTorrentPluginAdded(core::TorrentPlugin *plugin) {

    added();

    if(_request.paused) {
        finished();
        return;
    }

    auto torrents = _node->torrents();

    if(torrents.find(_request.torrentIdentifier.infoHash()) == torrents.end()) {
        finished(TorrentAddResponse::Error::TorrentDoesNotExist);
        return;
    }

    torrents[_request.torrentIdentifier.infoHash()]->resume([this](const std::exception_ptr &e) {
        if(e) {
            finished(TorrentAddResponse::Error::ResumeFailed);
        } else {
            finished();
        }
    });
}

void TorrentAdder::addTorrentCallback(libtorrent::error_code &ec, libtorrent::torrent_handle &th)
{
    if(ec) {
        std::cout << "addTorrent failed: " << ec.message().c_str() << std::endl;
        finished(ec);
    }
}

}
}
