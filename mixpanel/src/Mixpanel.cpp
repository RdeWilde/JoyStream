/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Saturday 14 November 2015
 */

#include <Mixpanel/Mixpanel.hpp>
#include <Mixpanel/Event.hpp>

#include <QNetworkReply>
#include <QEventLoop>
#include <QNetworkAccessManager>

namespace Mixpanel {

    bool sendEvent(const Event & event) {

        QNetworkAccessManager manager;

        QNetworkRequest request = event.toTrackingRequest();

        QNetworkReply * reply = manager.get(request);

        QEventLoop loop;

        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        loop.exec();

        QNetworkReply::NetworkError error = reply->error();

        if(error == QNetworkReply::NetworkError::NoError) {

            QByteArray response = reply->readAll();

            if(response.size() == 0)
                return false;
            else {

                bool ok;
                uint x = response.toUInt(&ok);

                if(!ok)
                    return false;
                else
                    return x == 1;
            }

        } else
            return false;
    }

}
