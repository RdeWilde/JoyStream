#include <app_kit/TorrentAdder.hpp>

namespace joystream {
namespace appkit {

TorrentAdder::TorrentAdder(core::Node* node,
                           core::TorrentIdentifier ti,
                           int downloadLimit, int uploadLimit,
                           std::vector<char> resumeData, std::string savePath)
    :  _node(node),
       _downloadLimit(downloadLimit),
       _uploadLimit(uploadLimit),
       _resumeData(resumeData),
       _savePath(savePath),
       _torrentIdentifier(ti)
{

    QObject::connect(_node, &joystream::core::Node::addedTorrent, this, &TorrentAdder::torrentAdded);
    QObject::connect(_node, &joystream::core::Node::removedTorrent, this, &TorrentAdder::torrentRemoved);
}

void TorrentAdder::add()
{
    try {
        _node->addTorrent(_downloadLimit, _uploadLimit,
                          libtorrent::to_hex(_torrentIdentifier.infoHash().to_string()),
                          _resumeData,
                          _savePath, true, _torrentIdentifier,
                          [this](libtorrent::error_code &ecode, libtorrent::torrent_handle &th) {
            addTorrentCallback(ecode, th);
        });

    } catch(std::exception &e) {
        emit addFailed(std::string(e.what()));
    }
}

void TorrentAdder::addTorrentCallback(libtorrent::error_code &ecode, libtorrent::torrent_handle &th)
{
    if(ecode) {
        std::cout << "addTorrent failed: " << ecode.message().c_str() << std::endl;

        // It would be more useful to emit different signals for different error codes
        emit addFailed(ecode.message().c_str());
    }
}

void TorrentAdder::torrentAdded(core::Torrent *torrent) {
    // Only handle our torrent
    if (torrent->infoHash() != _torrentIdentifier.infoHash())
        return;

    _torrent = torrent;

    // wait for torrent plugin to be added
    QObject::connect(_torrent, &joystream::core::Torrent::torrentPluginAdded, this, &TorrentAdder::torrentPluginAdded);

    emit added();
}

void TorrentAdder::torrentRemoved(const libtorrent::sha1_hash &info_hash) {
    _torrent = nullptr;
}

void TorrentAdder::torrentPluginAdded(core::TorrentPlugin *plugin) {

    QObject::connect(_torrent, &joystream::core::Torrent::stateChanged, this, &TorrentAdder::torrentStateChanged);

    emitState();

    _torrent->resume([this](const std::exception_ptr &e) {
        if(e) {
            emit resumeFailed();
        } else {
            emit resumed();
        }
    });
}

void TorrentAdder::torrentStateChanged(libtorrent::torrent_status::state_t state, float progress) {
    emitState();
}

void TorrentAdder::emitState() {
  switch(_torrent->state()) {
    case libtorrent::torrent_status::state_t::checking_files :
        emit checkingFiles();
        break;
    case libtorrent::torrent_status::state_t::downloading_metadata :
        emit downloadingMetaData();
        break;
    case libtorrent::torrent_status::state_t::downloading:
        emit downloading();
        break;
    case libtorrent::torrent_status::state_t::finished:
        emit finished();
        break;
    case libtorrent::torrent_status::state_t::seeding:
        emit seeding();
        break;
    case libtorrent::torrent_status::state_t::allocating:
        emit allocating();
        break;
    case libtorrent::torrent_status::state_t::checking_resume_data:
        emit checkingResumeData();
        break;
  }
}

}
}
