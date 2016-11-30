/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 29 2016
 */

#include <Test.hpp>
#include <extension/extension.hpp>

using namespace joystream;
using namespace joystream::extension;

Test::initTestCase() {

}

// currently: we are not requesting what pieces to read from session
// how to introduce gmock?
// Create MockPieceReadHandler using GMock

// q1: can we mock session_handle: yes, but should we use sessionInterface?
// q2: can we create read_piece_alert: yes,

void Test::pieceReader() {

    // * PieceReader

    // check that pieceReader is not set to begin with
    // add session handle
    // check that pieceReader is now set

}

void Test::readPiece() {

    // * readPiece
    // Create two MockPieceReadHandler object
    // make PieceReader::readPiece() for a few different pieces
    // HandlerAlreadyRegisteredWithPiece
    // Make alerts and send them to plugin, make one alert
    // which is irrelevant, one which is a piece no one requested
    // assert that handler was called with correct piece
}

void Test::cancelRequests() {

    // * cancelRequests
    // do the same again, cancel requests for one mock object
    // send alert for that piece
    // assert that nothing happened
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
