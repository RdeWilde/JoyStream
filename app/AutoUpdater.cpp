/* Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, September 2015
 */

#include <AutoUpdater.hpp>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QProcess>
#include <iostream>
#include <QFile>

AutoUpdater::AutoUpdater(QApplication& app)
    : _app(app)
{

}

/* does an unattended check - returns true if a new version is available */
bool AutoUpdater::newVersionAvailable() {
    QString program = updaterPath();
    QStringList arguments;
    arguments << "--mode" << "unattended";

    //avoid case on windows if updater is not found or fails to run to return true
    if( !QFile::exists(program)){
        return false;
    }

    std::cout << "checking for updates..." << std::endl;

    QProcess *process = new QProcess();

    QObject::connect(process, SIGNAL(finished(int)), this, SLOT(updateCheckDone(int)));

    process->start(program, arguments);

    _app.exec();

    //exit code 0 means an update is available to download
    return process->exitCode() == 0;
}

void AutoUpdater::updateCheckDone(int exitCode) {
    _app.exit();
}

/* runs auto updater in minimal gui - it will only show up if a new version is available to download
 * use on application start up or after newVersionAvailable() returns true ?
 */
void AutoUpdater::updateMiniUI(){
    QString program = updaterPath();
    QStringList arguments;
    arguments << "--mode" << "unattended";
    arguments << "--unattendedmodebehavior" << "download";
    arguments << "--unattendedmodeui" << "minimalWithDialogs";

    QProcess *process = new QProcess();
    process->start(program, arguments);
}

/* runs fully interactive auto-updater gui*/
/*
void AutoUpdater::updateFullUI(QApplication* app) {
    QObject *parent = 0;
    QString program = "open";
    QStringList arguments;
    arguments << _app.applicationDirPath() + "/../../../autoupdate-osx.app";

    QProcess *updaterProcess = new QProcess(parent);
    updaterProcess->start(program, arguments);
}

void AutoUpdater::updateFullUI(){
  //use _app.applicationDirPath() + "/../../../autoupdate-osx.app"; for OS X
  QDesktopServices::openUrl(QUrl("file:///" + updaterPath(), QUrl::TolerantMode));
}
*/

QString AutoUpdater::updaterPath(){
#ifdef _WIN32
    return _app.applicationDirPath() + "/autoupdate-windows.exe";
#elif __APPLE__
    return _app.applicationDirPath() + "/../../../autoupdate-osx.app/Contents/MacOs/installbuilder.sh";
#elif __linux
    return _app.applicationDirPath() + "/../autoupdate-linux.run";
#endif
}
