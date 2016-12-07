#ifndef TORRENTADDER_HPP
#define TORRENTADDER_HPP

#include <core/Node.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Torrent.hpp>
#include <core/TorrentPlugin.hpp>

#include <app_kit/Worker.hpp>
#include <app_kit/AddTorrentRequest.hpp>
#include <app_kit/WorkerResult.hpp>

namespace joystream {
namespace appkit {

class TorrentAdder : public Worker {

public:
    // TorrentAdder does not have a public API. It is always created on the heap and manages its own lifetime
    // It will be destoyed when a torrent add operation is finished. The WorkerResult
    // will hold the result of the operation.
    static std::shared_ptr<WorkerResult> add(QObject*, core::Node*, AddTorrentRequest);

signals:


protected slots:
    // slots are connected to signals from core::Node
    void onTorrentAdded(joystream::core::Torrent * torrent);
    void onTorrentPluginAdded(joystream::core::TorrentPlugin *plugin);

    void start();

private:
    // TorrentAdder manages its own lifetime so we restrict it from being created on the stack
    TorrentAdder(QObject*, core::Node*, AddTorrentRequest, std::shared_ptr<WorkerResult>);

    core::Node* _node;
    AddTorrentRequest _request;

    void addTorrentCallback(libtorrent::error_code &ecode, libtorrent::torrent_handle &th);
};

}
}
#endif // TORRENTADDER_HPP
