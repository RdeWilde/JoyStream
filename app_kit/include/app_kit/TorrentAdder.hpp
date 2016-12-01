#ifndef TORRENTADDER_HPP
#define TORRENTADDER_HPP

#include <core/Node.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Torrent.hpp>
#include <core/TorrentPlugin.hpp>

#include <app_kit/AppKit.hpp>
#include <app_kit/TorrentAddResponse.hpp>

namespace joystream {
namespace appkit {

class TorrentAdder : public QObject {
    Q_OBJECT

public:
    // TorrentAdder does not have a public API. It is always created on the heap and manages its own lifetime
    // It will be destoyed when it completes a torrent add operation is finished. The TorrentAddResponse
    // will hold the result of the operation.
    static std::shared_ptr<TorrentAddResponse> add(AppKit*, core::TorrentIdentifier, int downloadLimit, int uploadLimit, std::string name, const std::vector<char> &resumeData, std::string savePath, bool paused);

signals:


protected slots:
    // slots are connected to signals from core::Node
    void onTorrentAdded(joystream::core::Torrent * torrent);
    void onTorrentRemoved(const libtorrent::sha1_hash & info_hash);
    void onTorrentPluginAdded(joystream::core::TorrentPlugin *plugin);

private:
    // TorrentAdder manages its own lifetime so we restrict it from being created on the stack
    TorrentAdder(AppKit*, std::shared_ptr<TorrentAddResponse>, core::TorrentIdentifier, int downloadLimit, int uploadLimit, std::string name, const std::vector<char> &resumeData, std::string savePath, bool paused);

    libtorrent::sha1_hash _infoHash;
    bool _addPaused;
    std::shared_ptr<TorrentAddResponse> _response;
    core::Torrent *_torrent;

    // Updated response objects

    // Updated the response object to indicate that the torrent and torrentPlugin were added successfully
    void added();

    // Add operation completed normally
    void finished();

    // An Error occured while adding the torrent
    void finished(TorrentAddResponse::Error);

    // An Error occured while adding the torrent
    void finished(libtorrent::error_code);

    void addTorrentCallback(libtorrent::error_code &ecode, libtorrent::torrent_handle &th);
};

}
}
#endif // TORRENTADDER_HPP
