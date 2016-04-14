/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 14 2016
 */

#ifndef TEST_H
#define TEST_H

#include <QtTest/QtTest>

class Test : public QObject {

    Q_OBJECT

private slots:

    void buy();
    void sell();
    void fullPiece();
    void joinContract();
    void joiningContract();
    void payment();
    void ready();

};

#endif // TEST_H
