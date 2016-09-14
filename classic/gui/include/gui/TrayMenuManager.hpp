/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_TRAYMENUMANAGER_HPP
#define JOYSTREAM_CLASSIC_GUI_TRAYMENUMANAGER_HPP

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

namespace joystream {
namespace classic {
namespace gui {

class TrayMenuManager : public QObject
{
    Q_OBJECT

public:

    TrayMenuManager(QObject * parent);

public slots:

    void setTrayMenuVisiblity(bool visibile);

    void setTrayIconVisibility(bool visible);

signals:

    void openJoyStreamClicked();

    void minimizeToTrayClicked();

    void quitClicked();

private:

    // Menu actions in _trayIconContextMenu
    QAction _openJoyStream,
            _minimizeToTray,
            _quit;

    // Context menu for tray
    QMenu _trayIconContextMenu;

    // Icon placed in system tray
    QSystemTrayIcon _trayIcon;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_TRAYMENUMANAGER_HPP
