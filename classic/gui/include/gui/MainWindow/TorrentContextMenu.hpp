/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 18 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_TORRENTCONTEXTMENU_HPP
#define JOYSTREAM_CLASSIC_GUI_TORRENTCONTEXTMENU_HPP

#include <QObject>
#include <QMenu>
#include <QAction>

namespace joystream {
namespace protocol_session {
    enum class SessionMode;
}
namespace classic {
namespace gui {

class Torrent;

class TorrentContextMenu : public QMenu {

    Q_OBJECT

public:

    TorrentContextMenu(QWidget * parent,
                       bool torrentPluginActionsVisible,
                       bool paused,
                       const protocol_session::SessionMode & mode);

public slots:

    // Whether to show torrent plugin related actions
    void setTorrentPluginActionVisibility(bool visible = true);

    // Whether torrent is currently paused
    void setPauseStatus(bool paused);

    // Which mode the session is currently in
    void setSessionMode(const protocol_session::SessionMode & mode);

    // Expose menu actions
    QAction * manageSessionAction() const noexcept;
    QAction * viewPeersAction() const noexcept;
    QAction * removeTorrentAction() const noexcept;
    QAction * sessionToSellingModeAction() const noexcept;
    QAction * sessionToBuyingModeAction() const noexcept;
    QAction * sessionToObservingModeAction() const noexcept;
    QAction * pauseAction() const noexcept;
    QAction * startAction() const noexcept;
    QAction * streamMediaAction() const noexcept;
    QAction * openFolderLocationAction() const noexcept;

private:

    QActionGroup * sessionModeActionGroup() const noexcept;

    QActionGroup * stateActionGroup() const noexcept;

};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_TORRENTCONTEXTMENU_HPP
