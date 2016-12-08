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
    static std::shared_ptr<WorkerResult> observe(QObject* parent, core::Node*, libtorrent::sha1_hash, protocol_session::SessionState);

protected slots:
    void start();

private:
    TorrentObserver(QObject* parent, core::Node*, libtorrent::sha1_hash, protocol_session::SessionState, std::shared_ptr<WorkerResult> result);

    const protocol_session::SessionState _state;
};

}
}

#endif // TORRENTOBSERVER_HPP
