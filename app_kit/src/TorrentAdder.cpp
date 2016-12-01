#include <app_kit/TorrentAdder.hpp>
#include <core/Exception.hpp>

namespace joystream {
namespace appkit {

TorrentAdder::TorrentAdder(core::Node* node,
                           core::TorrentIdentifier ti,
                           int downloadLimit, int uploadLimit,
                           std::string name,
                           const std::vector<char> &resumeData, std::string savePath, bool paused)
    :  _node(node),
       _downloadLimit(downloadLimit),
       _uploadLimit(uploadLimit),
       _name(name),
       _resumeData(resumeData),
       _savePath(savePath),
       _torrentIdentifier(ti),
       _response(new TorrentAddResponse(ti)),
       _addPaused(paused)
{

    QObject::connect(_node, &joystream::core::Node::addedTorrent, this, &TorrentAdder::torrentAdded);
    QObject::connect(_node, &joystream::core::Node::removedTorrent, this, &TorrentAdder::torrentRemoved);

    try {
        _node->addTorrent(_uploadLimit, _downloadLimit,
                          _name == "" ? libtorrent::to_hex(_torrentIdentifier.infoHash().to_string()) : _name,
                          _resumeData,
                          _savePath, _addPaused, _torrentIdentifier,
                          [this](libtorrent::error_code &ecode, libtorrent::torrent_handle &th) {
            addTorrentCallback(ecode, th);
        });

    } catch (core::exception::TorrentAlreadyExists &e) {
        finishedWithError(TorrentAddResponse::Error::TorrentAlreadyExists);
    }
}

std::shared_ptr<TorrentAddResponse> TorrentAdder::add(core::Node *node, core::TorrentIdentifier ti, int downloadLimit, int uploadLimit, std::string name, const std::vector<char> &resumeData, std::string savePath, bool paused)
{
    auto adder = new TorrentAdder(node, ti, downloadLimit, uploadLimit, name, resumeData, savePath, paused);

    auto response = adder->response();

    QObject::connect(adder, &TorrentAdder::finished, response.get(), &TorrentAddResponse::finishedProcessing);

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

    emit finished();

    delete this;
}

void TorrentAdder::finishedSuccessfully()
{
    _response->setSuccess();

    emit finished();

    delete this;
}

void TorrentAdder::torrentAdded(core::Torrent *torrent) {

    if (torrent->infoHash() != _torrentIdentifier.infoHash())
        return;

    _torrent = torrent;

    // wait for torrent plugin to be added
    QObject::connect(_torrent, &joystream::core::Torrent::torrentPluginAdded, this, &TorrentAdder::torrentPluginAdded);
}

void TorrentAdder::torrentRemoved(const libtorrent::sha1_hash &info_hash) {
    if(_torrentIdentifier.infoHash() != info_hash)
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
