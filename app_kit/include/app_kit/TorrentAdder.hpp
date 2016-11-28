#ifndef TORRENTADDER_HPP
#define TORRENTADDER_HPP

#include <core/Node.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Torrent.hpp>
#include <core/TorrentPlugin.hpp>

namespace joystream {
namespace appkit {

class TorrentAdder : public QObject {
    Q_OBJECT

public:
    TorrentAdder(core::Node*, core::TorrentIdentifier, int downloadLimit, int uploadLimit, std::vector<char> resumeData, std::string savePath);

    // Tries to add the torrent to the node
    void add();

signals:
    void added();
    void addFailed(std::string err);
    void resumed();
    void resumeFailed();

    // torrent state signals
    void checkingFiles();
    void downloadingMetaData();
    void downloading();
    void finished();
    void seeding();
    void allocating();
    void checkingResumeData();

public slots:
    // slots are connected to signals from core::Node
    void torrentAdded(joystream::core::Torrent * torrent);
    void torrentRemoved(const libtorrent::sha1_hash & info_hash);
    void torrentPluginAdded(joystream::core::TorrentPlugin *plugin);
    void torrentStateChanged(libtorrent::torrent_status::state_t state, float progress);

private:
    core::Node* _node;
    int _downloadLimit;
    int _uploadLimit;
    std::vector<char> _resumeData;
    std::string _savePath;
    core::TorrentIdentifier _torrentIdentifier;

    core::Torrent *_torrent;

    void addTorrentCallback(libtorrent::error_code &ecode, libtorrent::torrent_handle &th);
    void emitState();
};

}
}
#endif // TORRENTADDER_HPP
