#ifndef TORRENTADDER_HPP
#define TORRENTADDER_HPP

#include <core/Node.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/Torrent.hpp>
#include <core/TorrentPlugin.hpp>

#include <app_kit/AddTorrentRequest.hpp>
#include <app_kit/AddTorrentResponse.hpp>

namespace joystream {
namespace appkit {

class TorrentAdder : public QObject {
    Q_OBJECT

public:
    // TorrentAdder does not have a public API. It is always created on the heap and manages its own lifetime
    // It will be destoyed when a torrent add operation is finished. The AddTorrentResponse
    // will hold the result of the operation.
    static std::shared_ptr<AddTorrentResponse> add(QObject*, core::Node*, AddTorrentRequest);

signals:


protected slots:
    // slots are connected to signals from core::Node
    void onTorrentAdded(joystream::core::Torrent * torrent);
    void onTorrentRemoved(const libtorrent::sha1_hash & info_hash);
    void onTorrentPluginAdded(joystream::core::TorrentPlugin *plugin);

private:
    // TorrentAdder manages its own lifetime so we restrict it from being created on the stack
    TorrentAdder(QObject*, core::Node*, AddTorrentRequest, std::shared_ptr<AddTorrentResponse>);

    static std::map<libtorrent::sha1_hash, TorrentAdder*> _workers;

    core::Node* _node;
    AddTorrentRequest _request;
    std::shared_ptr<AddTorrentResponse> _response;

    Q_INVOKABLE void start();
    Q_INVOKABLE void abort();

    // Updated the response object to indicate that the torrent and torrentPlugin were added successfully
    void added();

    // Add operation completed normally
    void finished();

    // An Error occured while adding the torrent
    void finished(AddTorrentResponse::Error);

    // An Error occured while adding the torrent
    void finished(libtorrent::error_code);

    void addTorrentCallback(libtorrent::error_code &ecode, libtorrent::torrent_handle &th);
};

}
}
#endif // TORRENTADDER_HPP
