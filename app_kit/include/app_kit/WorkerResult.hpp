#ifndef WORKER_RESULT_HPP
#define WORKER_RESULT_HPP

#include <QObject>
#include <libtorrent/sha1_hash.hpp>

namespace joystream {
namespace appkit {

class WorkerResult : public QObject
{
    Q_OBJECT
public:
    enum class Error {
        NoError,
        TorrentAlreadyExists,
        TorrentAlreadyBeingAdded,
        TorrentDoesNotExist,
        TorrentRemovedBeforePluginWasAdded,
        ResumeFailed,
        LibtorrentError,
        AlreadyTryingToBuyTorrent,
        TorrentAlreadyInBuySession,
        TorrentPluginNotSet,
        WalletLocked,
        TorrentAlreadyDownloaded,
        UnableToLockFunds,
        AlreadyTryingToSellTorrent,
        TorrentAlreadyInSellSession,
        TorrentNotFullyDownloadedYet,
        Exception
    };

    explicit WorkerResult(const libtorrent::sha1_hash infoHash)
        : _finished(false),
          _infoHash(infoHash),
          _err(Error::NoError),
          _eptr(std::exception_ptr(nullptr)){
    }

    void setError(Error err) { _err = err; emit error(_err, _ec, _eptr); }
    void setError(libtorrent::error_code ec) { _err = Error::LibtorrentError; _ec = ec; emit error(_err, _ec, _eptr); }
    void setError(std::exception_ptr eptr) { _err = Error::Exception; _eptr = eptr; emit error(_err, _ec, _eptr); }

    Error getError() const { return _err; }

    libtorrent::error_code errorCode() const { return _ec; }

    std::exception_ptr exceptionPtr() const { return _eptr; }

    libtorrent::sha1_hash infoHash() const { return _infoHash; }

    bool isFinished() const { return _finished; }

signals:
    void error(Error, libtorrent::error_code, std::exception_ptr);
    void finished();

public slots:
    void finishedProcessing() {
        _finished = true;
        emit finished();
    }

private:
    bool _finished;
    libtorrent::sha1_hash _infoHash;
    Error _err;
    libtorrent::error_code _ec;
    std::exception_ptr _eptr;
};

}
}

#endif // WORKER_RESULT_HPP
