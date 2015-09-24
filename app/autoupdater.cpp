#include "autoupdater.h"
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QProcess>
#include <iostream>

AutoUpdater::AutoUpdater(QApplication& app)
    : _app(app)
{

}

/* does an unattended check (blocking) - returns true if a new version is available */
bool AutoUpdater::newVersionAvailable() {
    QObject *parent = 0;
    QString program = updaterPath();
    QStringList arguments;
    arguments << "--mode" << "unattended";

    QProcess *process = new QProcess(parent);
    process->start(program, arguments);
    //blocking main thread..
    //Todo: let the updater finish running and emit a signal with exit code instead
    process->waitForFinished(-1);

    //exit code 0 means an update is available to download
    return process->exitCode() == 0;
}

/* runs auto updater in minimal gui - it will only show up if a new version is available to download
 * use on application start up or after newVersionAvailable() returns true ?
 */
void AutoUpdater::updateMiniUI(){
    QObject *parent = 0;
    QString program = updaterPath();
    QStringList arguments;
    arguments << "--mode" << "unattended";
    arguments << "--unattendedmodebehavior" << "download";
    arguments << "--unattendedmodeui" << "minimalWithDialogs";

    QProcess *process = new QProcess(parent);
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
    return QString("installebuilder.sh");//figure out location for linux
#endif
}
