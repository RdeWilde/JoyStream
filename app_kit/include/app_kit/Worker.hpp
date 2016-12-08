#ifndef WORKER_HPP
#define WORKER_HPP

#include <QObject>
#include <map>
#include <libtorrent/sha1_hash.hpp>
#include <app_kit/WorkerResult.hpp>
#include <core/Node.hpp>
#include <core/Torrent.hpp>
#include <core/TorrentPlugin.hpp>

namespace joystream {
namespace appkit {

class Worker : public QObject
{
    Q_OBJECT

public:
    Worker(QObject* parent, libtorrent::sha1_hash, std::shared_ptr<WorkerResult>, core::Node*);
    ~Worker();

    libtorrent::sha1_hash infoHash() const;

    core::Node* node() const;
    core::Torrent* getTorrent() const;
    void startPlugin();

    void finished();
    void finished(WorkerResult::Error);
    void finished(std::exception_ptr);
    void finished(libtorrent::error_code);

protected slots:
    virtual void start() = 0;
    void abort();
    void finishIfTorrentRemoved(const libtorrent::sha1_hash & info_hash);

private:
    static std::map<libtorrent::sha1_hash, const Worker*> _workers;
    const libtorrent::sha1_hash _infoHash;
    std::shared_ptr<WorkerResult> _result;
    core::Node* const _node;
};

}
}

#endif // WORKER_HPP
