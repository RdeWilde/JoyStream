/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Saturday 14 November 2015
 */

#ifndef MIXPANEL_EVENT_HPP
#define MIXPANEL_EVENT_HPP

#include <QMap>

class QHostAddress;
class QJsonObject;
class QDateTime;
class QNetworkRequest;

namespace Mixpanel {

    class Event {

    public:

        class Properties : public QMap<QString, QString> {

        public:

            Properties(const QString & token);

            QJsonObject toJSON() const;

            //QString distinctId() const;
            void setDistinctId(const QString & distinctId);

            //QDateTime time() const;
            void setTime(const QDateTime & time);

            //QHostAddress ip() const;
            void setIp(const QHostAddress & ip);

        private:

            // API token
            QString _token;
        };

        //Event(const QString & event);
        //Event(const QJsonObject & event);

        Event(const QString & event, const Properties & properties);

        // Request for tracking event
        QNetworkRequest toTrackingRequest() const;

        // JSON encoding of event
        QJsonObject toJSON() const;

        // Base64 encoded version of toJSON()
        QString toData() const;

        // Getters and setters
        Properties properties() const;
        void setProperties(const Properties & properties);

    protected:

        // Name of event
        QString _event;

        // Properties that are not covered by the ones above
        Properties _properties;
    };

}

#endif // MIXPANEL_EVENT_HPP
