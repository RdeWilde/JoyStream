/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QObject>

class Test : public QObject
{
    Q_OBJECT

public:

    static QString MixpanelToken;

private slots:

    void sendTrackerEvent();

};

#endif // TEST_HPP
