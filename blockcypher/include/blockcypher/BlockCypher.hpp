/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#ifndef BLOCKCYPHER_HPP
#define BLOCKCYPHER_HPP

#include <QtDebug>
#include <QNetworkReply> // cannot forward declare

class QJsonObject;
class QNetworkReply;

namespace blockcypher {

    // Base class for managing signals from networkreply objects for api requests
    class Reply : public QObject {

        Q_OBJECT

    public:

        Reply(QNetworkReply * reply);

        ~Reply();

        QNetworkReply * reply() const;

        QByteArray rawResponse() const;

        QNetworkReply::NetworkError error() const;

        QString errorReport() const;

        virtual const char * errorMessage() const = 0;

    // Apparently virtual slots from
    // pure virtual routines in base class need not
    // redeclare as slots:
    // http://stackoverflow.com/questions/2998216/does-qt-support-virtual-pure-slots
    public slots:

        //void QNetworkReplyError(QNetworkReply::NetworkError code);

        // Process
        void QNetworkReplyFinished();

        //
        virtual void processReply() = 0;

    public:

        // Reply object associated with operation
        QNetworkReply * _reply;

        // Response which comes from
        QByteArray _rawResponse;

        // The error assocaited with reply
        QNetworkReply::NetworkError _error;
    };

    /**
    // Parse reply into json
    QJsonObject replyToQJsonObject(QNetworkReply * reply);
    */

    // Parse raw buffer into json
    QJsonObject rawToQJsonObject(const QByteArray & data);

}

#endif // BLOCKCYPHER_HPP

