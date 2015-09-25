/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2015
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>
#include <QNetworkAccessManager>

class Test : public QObject
{
    Q_OBJECT

private slots:

    void init();

    /**
     * Test routines
     */

    void download_and_streaming();

    void paid_uploading();


private:

    // Manages HTTP access
    QNetworkAccessManager _manager;
};

#endif // TEST_HPP
