/* Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, September 2015
 */

#ifndef AUTO_UPDATER_HPP
#define AUTO_UPDATER_HPP

#include <QObject>
#include <QApplication>

namespace joystream {
namespace app {

    class AutoUpdater : QObject {

        Q_OBJECT

        QApplication& _app;

    public:
        AutoUpdater(QApplication&);
        bool newVersionAvailable();
        void updateMiniUI();
        //void updateFullUI();
        QString updaterPath();

    private slots:
        void updateCheckDone(int exitCode);
};

}
}

#endif // AUTO_UPDATER_HPP
