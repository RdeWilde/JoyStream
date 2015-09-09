/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#ifndef BLOCKCYPHER_HPP
#define BLOCKCYPHER_HPP

#include <QNetworkReply> // cannot forward declare

class QJsonObject;
class QNetworkReply;

namespace BlockCypher {

    // Base class for managing signals from networkreply objects for api requests
    class Reply : public QObject {

        Q_OBJECT

    public:

        Reply(QNetworkReply * reply);
        ~Reply();

        QNetworkReply * reply() const;

    public slots:

        virtual void QNetworkReplyError(QNetworkReply::NetworkError code) = 0;

        virtual void QNetworkReplyFinished() = 0;

    protected:

        // Reply object associated with operation
        QNetworkReply * _reply;
    };

    // Parse reply into json
    QJsonObject replyToQJsonObject(QNetworkReply * reply);

}

#endif // BLOCKCYPHER_HPP

