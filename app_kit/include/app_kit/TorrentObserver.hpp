#ifndef TORRENTOBSERVER_HPP
#define TORRENTOBSERVER_HPP

#include <app_kit/Worker.hpp>
#include <app_kit/WorkerResult.hpp>
#include <core/Node.hpp>
#include <core/Torrent.hpp>

namespace joystream {
namespace appkit {

class TorrentObserver : Worker
{
public:
    static std::shared_ptr<WorkerResult> observe(QObject* parent, core::Node*, libtorrent::sha1_hash);

protected slots:
    void start();

private:
    TorrentObserver(QObject* parent, core::Node*, libtorrent::sha1_hash, std::shared_ptr<WorkerResult> result);

    core::Node* const _node;

    core::Torrent* getTorrent();
};

}
}

#endif // TORRENTOBSERVER_HPP
