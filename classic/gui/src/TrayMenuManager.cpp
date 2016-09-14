/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#include <gui/TrayMenuManager.hpp>

namespace joystream {
namespace classic {
namespace gui {

TrayMenuManager::TrayMenuManager(QObject * parent)
    : QObject(parent)
    , _openJoyStream("Open JoyStream", this)
    , _minimizeToTray("Minimize to tray", this)
    , _quit("Quit", this)
{
    /// Setup context menu
    _trayIconContextMenu.addAction(&_openJoyStream);
    _trayIconContextMenu.addAction(&_minimizeToTray);
    //_trayIconContextMenu.addSeparator();
    _trayIconContextMenu.addAction(&_quit);

    /// Setup tray icon
    QIcon icon(":/icon/trayicon.png");
    _trayIcon.setIcon(icon);
    _trayIcon.setContextMenu(&_trayIconContextMenu);
    _trayIcon.setToolTip("JoyStream");
    _trayIcon.show();

    // Connect actions signals directly to signals on this object
    QObject::connect(&_openJoyStream,
                     &QAction::triggered,
                     this,
                     &TrayMenuManager::openJoyStreamClicked);

    QObject::connect(&_minimizeToTray,
                     &QAction::triggered,
                     this,
                     &TrayMenuManager::minimizeToTrayClicked);

    QObject::connect(&_quit,
                     &QAction::triggered,
                     this,
                     &TrayMenuManager::quitClicked);
}

void TrayMenuManager::setTrayMenuVisiblity(bool visibile) {

    if(visibile)
        _trayIconContextMenu.show();
    else
        _trayIconContextMenu.hide();
}

void TrayMenuManager::setTrayIconVisibility(bool visible) {

    if(visible)
        _trayIcon.show();
    else
        _trayIcon.hide();
}

}
}
}
