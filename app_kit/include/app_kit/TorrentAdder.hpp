#ifndef TORRENTADDER_HPP
#define TORRENTADDER_HPP

#include <core/Node.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Torrent.hpp>
#include <core/TorrentPlugin.hpp>

#include <app_kit/TorrentAddResponse.hpp>

namespace joystream {
namespace appkit {

class TorrentAdder : public QObject {
    Q_OBJECT

public:
    static std::shared_ptr<TorrentAddResponse> add(core::Node*, core::TorrentIdentifier, int downloadLimit, int uploadLimit, std::string name, const std::vector<char> &resumeData, std::string savePath, bool paused);

signals:
    void finished();

protected slots:
    // slots are connected to signals from core::Node
    void torrentAdded(joystream::core::Torrent * torrent);
    void torrentRemoved(const libtorrent::sha1_hash & info_hash);
    void torrentPluginAdded(joystream::core::TorrentPlugin *plugin);

private:
    TorrentAdder(core::Node*, core::TorrentIdentifier, int downloadLimit, int uploadLimit, std::string name, const std::vector<char> &resumeData, std::string savePath, bool paused);

    core::Node* _node;
    int _downloadLimit;
    int _uploadLimit;
    std::string _name;
    std::vector<char> _resumeData;
    std::string _savePath;
    core::TorrentIdentifier _torrentIdentifier;
    core::Torrent *_torrent;
    bool _addPaused;
    std::shared_ptr<TorrentAddResponse> _response;

    void finishedWithError(TorrentAddResponse::Error);
    void finishedWithLibtorrentError(libtorrent::error_code);
    void finishedSuccessfully();

    std::shared_ptr<TorrentAddResponse> response() const;

    void addTorrentCallback(libtorrent::error_code &ecode, libtorrent::torrent_handle &th);
};

}
}
#endif // TORRENTADDER_HPP
