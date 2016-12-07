#ifndef WORKER_HPP
#define WORKER_HPP

#include <QObject>
#include <map>
#include <libtorrent/sha1_hash.hpp>
#include <app_kit/WorkerResult.hpp>

namespace joystream {
namespace appkit {

class Worker : public QObject
{
    Q_OBJECT

public:
    Worker(QObject* parent, libtorrent::sha1_hash, std::shared_ptr<WorkerResult>);
    ~Worker();

    libtorrent::sha1_hash infoHash() const;

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
};

}
}

#endif // WORKER_HPP
