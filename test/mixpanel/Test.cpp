/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 15 2015
 */

#include <Test.hpp>
#include <Mixpanel/Event.hpp>
#include <Mixpanel/Mixpanel.hpp>

#include <QtTest/QtTest>
#include <QNetworkAccessManager>

QString Test::MixpanelToken = "78f32eeff9af07339402fde79eac62c6";

void Test::sendTrackerEvent() {

    Mixpanel::Event::Properties properties(Test::MixpanelToken);
    properties["made_up_property"] = 77;

    Mixpanel::Event event("testing_event", properties);

    // Sent event
    bool worked = Mixpanel::sendEvent(event);

    QVERIFY(worked);
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
