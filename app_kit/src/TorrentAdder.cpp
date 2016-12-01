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

    QObject::connect(kit->node(), &joystream::core::Node::addedTorrent, this, &TorrentAdder::torrentAdded);
    QObject::connect(kit->node(), &joystream::core::Node::removedTorrent, this, &TorrentAdder::torrentRemoved);

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
        finishedWithError(TorrentAddResponse::Error::TorrentAlreadyExists);
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

void TorrentAdder::finishedWithLibtorrentError(libtorrent::error_code ec)
{
    _response->setLibtorrentErrorCode(ec);

    finishedWithError(TorrentAddResponse::Error::LibtorrentError);
}

void TorrentAdder::finishedWithError(TorrentAddResponse::Error err)
{
    _response->setError(err);

    delete this;
}

void TorrentAdder::finishedSuccessfully()
{
    _response->setAdded();

    delete this;
}

void TorrentAdder::torrentAdded(core::Torrent *torrent) {

    if (torrent->infoHash() != _infoHash)
        return;

    _torrent = torrent;

    // wait for torrent plugin to be added
    QObject::connect(_torrent, &joystream::core::Torrent::torrentPluginAdded, this, &TorrentAdder::torrentPluginAdded);
}

void TorrentAdder::torrentRemoved(const libtorrent::sha1_hash &info_hash) {
    if(info_hash != _infoHash)
        return;

    finishedWithError(TorrentAddResponse::Error::TorrentRemovedBeforePluginWasAdded);
}

void TorrentAdder::torrentPluginAdded(core::TorrentPlugin *plugin) {

    if(_addPaused) {
        finishedSuccessfully();
        return;
    }

    _torrent->resume([this](const std::exception_ptr &e) {
        if(e) {
            finishedWithError(TorrentAddResponse::Error::ResumeFailed);
        } else {
            finishedSuccessfully();
        }
    });
}

std::shared_ptr<TorrentAddResponse> TorrentAdder::response() const {
    return _response;
}

void TorrentAdder::addTorrentCallback(libtorrent::error_code &ec, libtorrent::torrent_handle &th)
{
    if(ec) {
        std::cout << "addTorrent failed: " << ec.message().c_str() << std::endl;
        finishedWithLibtorrentError(ec);
    }
}

}
}
