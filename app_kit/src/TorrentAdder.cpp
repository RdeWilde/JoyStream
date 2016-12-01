#include <app_kit/TorrentAdder.hpp>
#include <core/Exception.hpp>

namespace joystream {
namespace appkit {

TorrentAdder::TorrentAdder(appkit::AppKit* kit,
                           std::shared_ptr<TorrentAddResponse> response,
                           core::TorrentIdentifier ti,
                           int downloadLimit, int uploadLimit,
                           std::string name,
                           const std::vector<char> &resumeData, std::string savePath, bool paused)
    :  QObject(kit), // make the adder a child of appkit
       _infoHash(ti.infoHash()),
       _addPaused(paused),
       _response(response) {

    QObject::connect(kit->node(), &joystream::core::Node::addedTorrent, this, &TorrentAdder::onTorrentAdded);
    QObject::connect(kit->node(), &joystream::core::Node::removedTorrent, this, &TorrentAdder::onTorrentRemoved);

    QObject::connect(this, &TorrentAdder::destroyed, response.get(), &TorrentAddResponse::finishedProcessing);

    try {
        kit->node()->addTorrent(uploadLimit, downloadLimit,
                                name == "" ? libtorrent::to_hex(ti.infoHash().to_string()) : name,
                                resumeData,
                                savePath, _addPaused, ti,
                                [this](libtorrent::error_code &ecode, libtorrent::torrent_handle &th) {
            addTorrentCallback(ecode, th);
        });

    } catch (core::exception::TorrentAlreadyExists &e) {
        finished(TorrentAddResponse::Error::TorrentAlreadyExists);
    }
}

std::shared_ptr<TorrentAddResponse> TorrentAdder::add(appkit::AppKit* kit,
                                                      core::TorrentIdentifier ti,
                                                      int downloadLimit, int uploadLimit,
                                                      std::string name,
                                                      const std::vector<char> &resumeData, std::string savePath, bool paused)
{

    auto response = std::make_shared<TorrentAddResponse>(ti);

    new TorrentAdder(kit, response, ti, downloadLimit, uploadLimit, name, resumeData, savePath, paused);

    return response;
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

void TorrentAdder::finished(TorrentAddResponse::Error err)
{
    _response->setError(err);

    finished();
}

void TorrentAdder::added() {
    _response->setAdded();
}

void TorrentAdder::onTorrentAdded(core::Torrent *torrent) {

    if (torrent->infoHash() != _infoHash)
        return;

    _torrent = torrent;

    // wait for torrent plugin to be added
    QObject::connect(_torrent, &joystream::core::Torrent::torrentPluginAdded, this, &TorrentAdder::onTorrentPluginAdded);
}

void TorrentAdder::onTorrentRemoved(const libtorrent::sha1_hash &info_hash) {
    if(info_hash != _infoHash)
        return;

    finished(TorrentAddResponse::Error::TorrentRemovedBeforePluginWasAdded);
}

void TorrentAdder::onTorrentPluginAdded(core::TorrentPlugin *plugin) {

    added();

    if(_addPaused) {
        finished();
        return;
    }

    _torrent->resume([this](const std::exception_ptr &e) {
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
