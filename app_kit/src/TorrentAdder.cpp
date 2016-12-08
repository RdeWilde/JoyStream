#include <app_kit/TorrentAdder.hpp>
#include <core/Exception.hpp>

#include <QTimer>

namespace joystream {
namespace appkit {

TorrentAdder::TorrentAdder(QObject* parent,
                           core::Node* node,
                           AddTorrentRequest request,
                           std::shared_ptr<WorkerResult> result)
    :  Worker(parent, request.torrentIdentifier.infoHash(), result, node),
       _request(request)
{

}

std::shared_ptr<WorkerResult> TorrentAdder::add(QObject* parent,
                                                core::Node* node,
                                                AddTorrentRequest request)
{

    auto result = std::make_shared<WorkerResult>(request.torrentIdentifier.infoHash());

    new TorrentAdder(parent, node, request, result);

    return result;
}

void TorrentAdder::start() {

    QObject::connect(node(), &joystream::core::Node::addedTorrent, this, &TorrentAdder::onTorrentAdded);

    try {
        node()->addTorrent(_request.uploadLimit,
                         _request.downloadLimit,
                         _request.name == "" ? libtorrent::to_hex(_request.torrentIdentifier.infoHash().to_string()) : _request.name,
                         _request.resumeData,
                         _request.savePath, _request.paused, _request.torrentIdentifier,
                         [this](libtorrent::error_code &ecode, libtorrent::torrent_handle &th) {
            addTorrentCallback(ecode, th);
        });

    } catch (core::exception::TorrentAlreadyExists &e) {
        finished(WorkerResult::Error::TorrentAlreadyExists);
    }
}

void TorrentAdder::onTorrentAdded(core::Torrent *torrent) {

    if (torrent->infoHash() != infoHash())
        return;

    // wait for torrent plugin to be added
    QObject::connect(torrent, &joystream::core::Torrent::torrentPluginAdded, this, &TorrentAdder::onTorrentPluginAdded);
}

void TorrentAdder::onTorrentPluginAdded(core::TorrentPlugin *plugin) {

    auto torrents = node()->torrents();

    if(torrents.find(infoHash()) == torrents.end()) {
        finished(WorkerResult::Error::TorrentDoesNotExist);
        return;
    }

    // We always resume the torrent otherwise the torrent will not go to seeding state
    // if the torrent is fully available
    torrents[infoHash()]->resume([this](const std::exception_ptr &e) {
        if(e) {
            finished(WorkerResult::Error::ResumeFailed);
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
