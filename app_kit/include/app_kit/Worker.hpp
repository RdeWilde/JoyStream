#ifndef WORKER_HPP
#define WORKER_HPP

#include <QObject>
#include <map>
#include <libtorrent/sha1_hash.hpp>

namespace joystream {
namespace appkit {

class Worker : public QObject
{
    Q_OBJECT

public:
    Worker(QObject* parent, libtorrent::sha1_hash);
    ~Worker();

protected slots:
    virtual void start() = 0;
    virtual void abort() = 0;

private:
    static std::map<libtorrent::sha1_hash, Worker*> _workers;
    libtorrent::sha1_hash _infoHash;
};

}
}

#endif // WORKER_HPP
