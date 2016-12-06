#include <app_kit/TorrentAdder.hpp>
#include <core/Exception.hpp>

#include <QTimer>

namespace joystream {
namespace appkit {

TorrentAdder::TorrentAdder(QObject* parent,
                           core::Node* node,
                           AddTorrentRequest request,
                           std::shared_ptr<AddTorrentResponse> response)
    :  Worker(parent, request.torrentIdentifier.infoHash()),
       _node(node),
       _request(request),
       _response(response)
{
    QObject::connect(this, &TorrentAdder::destroyed, _response.get(), &AddTorrentResponse::finishedProcessing);
}

void TorrentAdder::abort()
{
    _response->setError(AddTorrentResponse::Error::TorrentAlreadyBeingAdded);
    delete this;
}

std::shared_ptr<AddTorrentResponse> TorrentAdder::add(QObject* parent,
                                                      core::Node* node,
                                                      AddTorrentRequest request)
{

    auto response = std::make_shared<AddTorrentResponse>(request.torrentIdentifier.infoHash());

    new TorrentAdder(parent, node, request, response);

    return response;
}

void TorrentAdder::start() {

    QObject::connect(_node, &joystream::core::Node::addedTorrent, this, &TorrentAdder::onTorrentAdded);

    QObject::connect(_node, &joystream::core::Node::removedTorrent, this, &TorrentAdder::onTorrentRemoved);

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
        finished(AddTorrentResponse::Error::TorrentAlreadyExists);
    }
}

void TorrentAdder::finished()
{
    delete this;
}

void TorrentAdder::finished(libtorrent::error_code ec)
{
    _response->setError(ec);

    finished();
}

void TorrentAdder::finished(AddTorrentResponse::Error err)
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

    finished(AddTorrentResponse::Error::TorrentRemovedBeforePluginWasAdded);
}

void TorrentAdder::onTorrentPluginAdded(core::TorrentPlugin *plugin) {

    added();

    auto torrents = _node->torrents();

    if(torrents.find(_request.torrentIdentifier.infoHash()) == torrents.end()) {
        finished(AddTorrentResponse::Error::TorrentDoesNotExist);
        return;
    }

    // We always resume the torrent otherwise the torrent will not go to seeding state
    // if the torrent is fully available
    torrents[_request.torrentIdentifier.infoHash()]->resume([this](const std::exception_ptr &e) {
        if(e) {
            finished(AddTorrentResponse::Error::ResumeFailed);
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
