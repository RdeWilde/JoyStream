/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 29 2016
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>

class Test : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase();

    /// PieceReaderPlugin

    void pieceReader();

    /// PieceReader

    void readPiece();

    void cancelRequests();
};

#endif // TEST_HPP
