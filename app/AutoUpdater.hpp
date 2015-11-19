/* Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, September 2015
 */

#ifndef AUTOUPDATER_H
#define AUTOUPDATER_H

#include <QObject>
#include <QApplication>

class AutoUpdater : QObject
{
    Q_OBJECT

    QApplication& _app;

public:
    AutoUpdater(QApplication&);
    bool newVersionAvailable();
    void updateMiniUI();
    //void updateFullUI();
    QString updaterPath();

signals:

public slots:
};

#endif // AUTOUPDATER_H
