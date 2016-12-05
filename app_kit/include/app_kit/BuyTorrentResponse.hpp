#ifndef BUYTORRENTRESPONSE_HPP
#define BUYTORRENTRESPONSE_HPP

#include <QObject>

#include <libtorrent/sha1_hash.hpp>

namespace joystream {
namespace appkit {


class BuyTorrentResponse : public QObject
{
    Q_OBJECT

public:
    enum class Error {
        NoError,
        TorrentDoesNotExist,
        AlreadyTryingToBuyTorrent,
        TorrentAlreadyInBuySession,
        TorrentPluginNotSet,
        WalletLocked,
        TorrentAlreadyDownloaded,
        UnableToLockFunds,
        //GoingToBuyModeFailed,
        //GeneralError,
        //FailedToStartPlugin
    };

    BuyTorrentResponse(libtorrent::sha1_hash infoHash)
      : _infoHash(infoHash),
        _finished(false),
        _err(Error::NoError),
        _eptr(std::exception_ptr(nullptr)){

    }

    void setError(Error err) { _err = err; emit error(_err, _eptr); }
    void setError(std::exception_ptr eptr) { _eptr = eptr; emit error(_err, _eptr); }

    Error getError() const { return _err; }

signals:
    void error(Error, std::exception_ptr);
    void finished();

public slots:
    void finishedProcessing() {
        _finished = true;
        emit finished();
    }

private:
    libtorrent::sha1_hash _infoHash;
    bool _finished;
    Error _err;
    std::exception_ptr _eptr;
};

}
}

#endif // BUYTORRENTRESPONSE_HPP
