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

    libtorrent::sha1_hash infoHash() const;

protected slots:
    virtual void start() = 0;
    virtual void abort() = 0;

private:
    static std::map<libtorrent::sha1_hash, const Worker*> _workers;
    const libtorrent::sha1_hash _infoHash;
};

}
}

#endif // WORKER_HPP
