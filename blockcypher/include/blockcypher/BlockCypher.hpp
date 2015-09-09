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

    // Apparently virtual slots from
    // pure virtual routines in base class need not
    // redeclare as slots:
    // http://stackoverflow.com/questions/2998216/does-qt-support-virtual-pure-slots
    public slots:

        //void QNetworkReplyError(QNetworkReply::NetworkError code);

        virtual void QNetworkReplyFinished() = 0;

    //signals:

        //void error(QNetworkReply::NetworkError code);

    protected:

        // Reply object associated with operation
        QNetworkReply * _reply;

        //QJsonObject _blockCypherError;
    };

    /**
    // Parse reply into json
    QJsonObject replyToQJsonObject(QNetworkReply * reply);
    */

    // Parse raw buffer into json
    QJsonObject rawToQJsonObject(const QByteArray & data);

}

#endif // BLOCKCYPHER_HPP

