#ifndef TORRENTADDRESPONSE_HPP
#define TORRENTADDRESPONSE_HPP

#include <QObject>
#include <core/TorrentIdentifier.hpp>

namespace joystream {
namespace appkit {

class TorrentAddResponse : public QObject
{
    Q_OBJECT
public:
    enum class Error {
        NoError,
        TorrentAlreadyExists,
        TorrentRemovedBeforePluginWasAdded,
        ResumeFailed,
        LibtorrentError
    };

    explicit TorrentAddResponse(const libtorrent::sha1_hash infoHash)
        : _finished(false),
          _added(false),
          _infoHash(infoHash),
          _err(Error::NoError) {
    }

    void setError(Error err) { _err = err; emit error(_err, _ec); }
    void setError(libtorrent::error_code ec) { _err = Error::LibtorrentError; _ec = ec; emit error(_err, _ec); }

    Error getError() const { return _err; }

    libtorrent::error_code errorCode() const { return _ec; }

    void setAdded() { _added = true; emit added(); }
    bool wasAdded() const { return _added; }

    libtorrent::sha1_hash infoHash() const { return _infoHash; }

    bool isFinished() const { return _finished; }

signals:
    void added();
    void error(Error, libtorrent::error_code);
    void finished();

public slots:
    void finishedProcessing() {
        _finished = true;
        emit finished();
    }

private:
    bool _finished;
    bool _added;
    libtorrent::error_code _ec;
    libtorrent::sha1_hash _infoHash;
    Error _err;
};

}
}

#endif // TORRENTADDRESPONSE_HPP
