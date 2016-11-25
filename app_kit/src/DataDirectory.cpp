/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 2016
 */

#include <app_kit/DataDirectory.hpp>

#include <QFileInfo>

namespace joystream {
namespace appkit {

const QString DataDirectory::LockFileName = ".lock";

DataDirectory::DataDirectory(const std::string &dir)
    : _dir(QDir(QString::fromStdString(dir))),
      _lockFile(_dir.absoluteFilePath(LockFileName)) {

    if(!_dir.exists()) {
        throw std::runtime_error("path to data directory doesn't exist");
    }

    QFileInfo fi(_dir.absolutePath());

    if(!fi.isWritable()) {
        throw std::runtime_error("data directory path is not writeable");
    }

    _lockFile.setStaleLockTime(0);

    lock();

    makeDirs();
}

void DataDirectory::lock() {

    if(!_lockFile.tryLock(10)) {
        throw std::runtime_error("unable to obtain a lock on the data directory");
    }
}

void DataDirectory::unlock() {
    _lockFile.unlock();
}

QLockFile::LockError DataDirectory::lockError() const {
     return _lockFile.error();
}

QString DataDirectory::walletPath() const {
    return _dir.absoluteFilePath("wallet");
}

QString DataDirectory::walletFilePath() const {
    return QDir(walletPath()).absoluteFilePath("store.sqlite");
}

QString DataDirectory::blockTreeFilePath() const {
    return QDir(walletPath()).absoluteFilePath("blocktree.dat");
}

QString DataDirectory::defaultSavePath() const {
    return _dir.absoluteFilePath("downloads");
}

QString DataDirectory::savedTorrentsFilePath() const {
    return _dir.absoluteFilePath("torrents.json");
}

void DataDirectory::makeDirs() const {
    _dir.mkdir(walletPath());
    _dir.mkpath(defaultSavePath());
}

}
}
