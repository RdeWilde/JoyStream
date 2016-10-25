#ifndef JOYSTREAM_DATADIRECTORY_HPP
#define JOYSTREAM_DATADIRECTORY_HPP

#include <QString>
#include <QDir>
#include <QLockFile>

namespace joystream {
namespace appkit {

class DataDirectory
{
public:
    static const QString LockFileName;

    DataDirectory(const QString &dir);

    void lock();

    void unlock();

    QLockFile::LockError lockError() const;

    QString walletPath() const;
    QString walletFilePath() const;
    QString blockTreeFilePath() const;
    QString nodeStatePath() const;
    QString defaultSavePath() const;

    void makeDirs() const;

private:
    QDir _dir;
    QLockFile _lockFile;
};

} // appkit
} // joystream

#endif // JOYSTREAM_DATADIRECTORY_HPP
